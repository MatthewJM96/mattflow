#include "stdafx.h"

#include "ast/node.h"
#include "literal/identifier.h"

#include "ast/parse.h"

static void link_operations_on_stack(
    VALOUT mfast::AST& ast,
    VALOUT mfast::NodeBuffers& nodes,
    VALOUT mfast::ParserState& parser_state
) {
    (void)ast;
    (void)nodes;
    (void)parser_state;
}

// TODO(Matthew): probably rename, need to differentiate control flow, operators, and
//                primaries at least.
static void add_non_operating_node(
    VALINOUT mflex::Tokens::const_iterator& curr_token,
    VALIN mfast::NodeInfo&& node_info,
    VALOUT mfast::AST& ast,
    VALOUT mfast::NodeBuffers& nodes,
    VALOUT mfast::ParserState& parser_state
) {
    // If we have two non-operating nodes in a row, we have a break of expression.
    // TODO(Matthew): bare in mind that we have control-flow to include yet, and might
    //                complicate this.
    if (parser_state.last_seen.back() != mfast::NodeCategory::BINOP
        || parser_state.last_seen.back() != mfast::NodeCategory::UNOP)
    {
        link_operations_on_stack(ast, nodes, parser_state);

        // TODO(Matthew): we kinda have to do more here as what if we have a parenexpr,
        //                blockexpr...
    }

    // Add vertex to AST for node, and push it onto the stack.
    auto vertex = boost::add_vertex(ast);
    parser_state.non_operating_vertices.back().emplace_back(vertex);

    // Add node info about bool node and associate with vertex in AST.
    nodes.vertex_node_map[vertex] = nodes.node_info.size();
    nodes.node_info.emplace_back(std::forward<mfast::NodeInfo>(node_info));

    // Move forward a token.
    curr_token += 1;
}

template <typename Node>
static void add_operating_node(
    VALINOUT mflex::Tokens::const_iterator& curr_token,
    VALIN Node&&                            node_info,
    VALOUT mfast::AST& ast,
    VALOUT mfast::NodeBuffers& nodes,
    VALOUT mfast::ParserState& parser_state
) {
    // TODO(Matthew): associativity needs to be handled here...
    //                  in particular it can only change if we have a chance of
    //                  precedence.
    // If operator has lower precedence than the highest we've so far encountered, we
    // need to deal with the expression as formed so far before continuing on. In
    // either case the current block/paren expr precedence must be overwritten with
    // this operator's precedence.
    if (node_info.PRECEDENCE < parser_state.precedence.back()) {
        link_operations_on_stack(ast, nodes, parser_state);
    }
    parser_state.precedence.back()    = node_info.PRECEDENCE;
    parser_state.associativity.back() = node_info.ASSOCIATIVITY;

    // Add vertex to AST for node, and push it onto the stack.
    auto vertex = boost::add_vertex(ast);
    parser_state.operating_vertices.back().emplace_back(vertex);

    // Add node info about bool node and associate with vertex in AST.
    nodes.vertex_node_map[vertex] = nodes.node_info.size();
    nodes.node_info.emplace_back(std::forward<Node>(node_info));

    // Move forward a token.
    curr_token += 1;
}

void mfast::parse(
    VALIN const mflex::Tokens& tokens,
    VALOUT AST&                ast,
    VALOUT NodeBuffers&        nodes,
    VALOUT mftype::IdentifierTypeTable& type_table
) {
    (void)type_table;

    // Ensure buffers are clear for building fresh AST.
    ast.clear();
    nodes.node_info.clear();
    nodes.vertex_node_map = boost::get(vertex_data, ast);
    nodes.node_info.emplace_back(BlockExprNode{ tokens.begin(), tokens.begin() });

    // Prepare stack for precedence, associativity, and last attachable vertex with the
    // top-level block node for module.
    ParserState parser_state;
    parser_state.precedence.emplace_back(Precedence::NONE);
    parser_state.associativity.emplace_back(Associativity::SENTINEL);
    parser_state.operating_vertices.push_back({});
    parser_state.non_operating_vertices.push_back({});
    parser_state.enclosing_vertices.push_back({});
    parser_state.enclosing_vertices.emplace_back(boost::add_vertex(ast));
    parser_state.non_operating_vertices.back().emplace_back(
        parser_state.enclosing_vertices.back()
    );
    parser_state.last_seen.emplace_back(NodeCategory::NONE);
    parser_state.enclosed_by.emplace_back(EnclosingCategory::ROOT);
    nodes.vertex_node_map[parser_state.enclosing_vertices.back()] = 0;

    // Iterate tokens, parsing as appropriate.
    auto it = tokens.begin();
    while (it != tokens.end()) {
        switch (it->type) {
            case mflex::TokenType::IF:
            case mflex::TokenType::THEN:
            case mflex::TokenType::ELIF:
            case mflex::TokenType::ELSE:
            case mflex::TokenType::FOR:
            case mflex::TokenType::IN:
            case mflex::TokenType::WHERE:
            case mflex::TokenType::WHILE:
            case mflex::TokenType::DO:
            case mflex::TokenType::MATCH:
            case mflex::TokenType::PRINT:
            case mflex::TokenType::STRUCT:
            case mflex::TokenType::LEFT_BRACE:
            case mflex::TokenType::RIGHT_BRACE:
            case mflex::TokenType::AND:
            case mflex::TokenType::OR:
            case mflex::TokenType::EQUALS:
            case mflex::TokenType::NOT_EQUALS:
            case mflex::TokenType::LESS_THAN:
            case mflex::TokenType::LESS_THAN_OR_EQUAL_TO:
            case mflex::TokenType::GREATER_THAN:
            case mflex::TokenType::GREATER_THAN_OR_EQUAL_TO:
            case mflex::TokenType::PLUS:
            case mflex::TokenType::MINUS:
                // TODO(Matthew): for now this token can be both repr of unary and
                //                binary operations (subtraction and negation).
            case mflex::TokenType::SLASH:
            case mflex::TokenType::STAR:
            case mflex::TokenType::IDENTIFIER:
            case mflex::TokenType::LEFT_PAREN:
            case mflex::TokenType::RIGHT_PAREN:
            case mflex::TokenType::LEFT_BRACKET:
            case mflex::TokenType::RIGHT_BRACKET:
            case mflex::TokenType::COMMA:
            case mflex::TokenType::RANGE:
            case mflex::TokenType::DOT:
            case mflex::TokenType::ARROW:
            case mflex::TokenType::ASSIGN_TYPE:
            case mflex::TokenType::ASSIGN_VALUE:
            case mflex::TokenType::SENTINEL:
                break;
            case mflex::TokenType::NOT:
                // Add NOT vertex.
                add_operating_node(
                    it, mfast::NotOperatorNode{ it, it }, ast, nodes, parser_state
                );
                continue;
            case mflex::TokenType::NIL:
                // Add null vertex.
                add_non_operating_node(
                    it, mfast::NullNode{ it, it }, ast, nodes, parser_state
                );
                continue;
            case mflex::TokenType::CHAR:
                // Add CHAR vertex.
                add_non_operating_node(
                    it, mfast::CharNode{ it, it }, ast, nodes, parser_state
                );
                continue;
            case mflex::TokenType::BOOL:
                // Add BOOL vertex.
                add_non_operating_node(
                    it, mfast::BoolNode{ it, it }, ast, nodes, parser_state
                );
                continue;
            case mflex::TokenType::INT:
                // Add INT vertex.
                add_non_operating_node(
                    it, mfast::IntNode{ it, it }, ast, nodes, parser_state
                );
                continue;
            case mflex::TokenType::INT8:
                // Add INT8 vertex.
                add_non_operating_node(
                    it, mfast::Int8Node{ it, it }, ast, nodes, parser_state
                );
                continue;
            case mflex::TokenType::INT16:
                // Add INT16 vertex.
                add_non_operating_node(
                    it, mfast::Int16Node{ it, it }, ast, nodes, parser_state
                );
                continue;
            case mflex::TokenType::INT32:
                // Add INT32 vertex.
                add_non_operating_node(
                    it, mfast::Int32Node{ it, it }, ast, nodes, parser_state
                );
                continue;
            case mflex::TokenType::INT64:
                // Add INT64 vertex.
                add_non_operating_node(
                    it, mfast::Int64Node{ it, it }, ast, nodes, parser_state
                );
                continue;
            case mflex::TokenType::UINT:
                // Add UINT vertex.
                add_non_operating_node(
                    it, mfast::UIntNode{ it, it }, ast, nodes, parser_state
                );
                continue;
            case mflex::TokenType::UINT8:
                // Add UINT8 vertex.
                add_non_operating_node(
                    it, mfast::UInt8Node{ it, it }, ast, nodes, parser_state
                );
                continue;
            case mflex::TokenType::UINT16:
                // Add UINT16 vertex.
                add_non_operating_node(
                    it, mfast::UInt16Node{ it, it }, ast, nodes, parser_state
                );
                continue;
            case mflex::TokenType::UINT32:
                // Add UINT32 vertex.
                add_non_operating_node(
                    it, mfast::UInt32Node{ it, it }, ast, nodes, parser_state
                );
                continue;
            case mflex::TokenType::UINT64:
                // Add UINT64 vertex.
                add_non_operating_node(
                    it, mfast::UInt64Node{ it, it }, ast, nodes, parser_state
                );
                continue;
            case mflex::TokenType::FLOAT32:
                // Add FP32 vertex.
                add_non_operating_node(
                    it, mfast::Float32Node{ it, it }, ast, nodes, parser_state
                );
                continue;
            case mflex::TokenType::FLOAT64:
                // Add FP64 vertex.
                add_non_operating_node(
                    it, mfast::Float64Node{ it, it }, ast, nodes, parser_state
                );
                continue;
            case mflex::TokenType::TRUE:
            case mflex::TokenType::FALSE:
                // Add Boolean value vertex.
                add_non_operating_node(
                    it,
                    mfast::BoolValNode{ it, it, it->type == mflex::TokenType::TRUE },
                    ast,
                    nodes,
                    parser_state
                );
                continue;
            case mflex::TokenType::NUMBER:
                // Add number vertex.
                add_non_operating_node(
                    it,
                    mfast::NumberValNode{ it, it, it->number },
                    ast,
                    nodes,
                    parser_state
                );
                continue;
            case mflex::TokenType::STRING:
                // Add string vertex.
                add_non_operating_node(
                    it,
                    mfast::StringValNode{ it, it, it->string_idx },
                    ast,
                    nodes,
                    parser_state
                );
                continue;
                // case mflex::TokenType::LEFT_PAREN:
                //     // Add paren vertex, push precedence parser_expects::ASSIGNMENT.
                //     add_paren_node(it, ast, nodes, parser_state);
                //     continue;
                // case mflex::TokenType::LEFT_BRACKET:
                //     // TODO(Matthew): ambiguity as could be number range or list.
                //     //                  need a way to decide which we are seeing
                //     here.
                //     //                  are we happy with this approach?
                //     if ((it + 2)->type == mflex::TokenType::RANGE) {
                //         // Add range vertex, push precedence parser_expects::PRIMARY.
                //         add_number_range_node(it, ast, nodes, parser_state);
                //     } else {
                //         // Add list vertex, push precedence
                //         parser_expects::EXPRESSION. add_list_node(it, ast, nodes,
                //         parser_state);
                //     }
                //     continue;
        }
    }

    // If we get here and cursor is not pointing to top-level block, then something has
    // gone wrong in parsing.
    // assert(parser_state.vertices.size() == 0);
}
