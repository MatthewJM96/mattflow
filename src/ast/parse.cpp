#include "stdafx.h"

#include "ast/link.h"
#include "ast/node.h"
#include "literal/identifier.h"

#include "ast/parse.h"

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
    if (parser_state.last_seen.back() != mfast::NodeCategory::NONE
        && parser_state.last_seen.back() != mfast::NodeCategory::BINOP
        && parser_state.last_seen.back() != mfast::NodeCategory::UNOP)
    {
        mfast::link_operations_on_stack(
            mfast::Precedence::NONE, ast, nodes, parser_state
        );

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
    // need to deal with the expression as formed so far (up to any preceding operation
    // with the same precedence as this to-be-added operation) before continuing on. In
    // either case the current block/paren expr precedence must be overwritten with
    // this operator's precedence.
    if (node_info.PRECEDENCE < parser_state.precedence.back()) {
        mfast::link_operations_on_stack(node_info.PRECEDENCE, ast, nodes, parser_state);
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
            case mflex::TokenType::LEFT_BRACKET:
            case mflex::TokenType::RIGHT_BRACKET:
            case mflex::TokenType::COMMA:
            case mflex::TokenType::RANGE:
            case mflex::TokenType::DOT:
            case mflex::TokenType::ARROW:
            case mflex::TokenType::SENTINEL:
                break;
            case mflex::TokenType::LEFT_PAREN:
                // Push a new stack of vertices for ops and non-ops.
                parser_state.non_operating_vertices.push_back({});
                parser_state.operating_vertices.push_back({});

                // Move forward a token.
                it += 1;
                continue;
            case mflex::TokenType::RIGHT_PAREN:
                // Link any remaining operators in the paren expression.
                link_operations_on_stack(Precedence::NONE, ast, nodes, parser_state);

                {
                    // Get root node of the paren expression and then pop the stack.
                    size_t paren_root
                        = parser_state.non_operating_vertices.back().back();
                    parser_state.non_operating_vertices.pop_back();
                    parser_state.operating_vertices.pop_back();

                    assert(parser_state.non_operating_vertices.size() > 0);
                    assert(parser_state.operating_vertices.size() > 0);

                    // Push root node onto the stack below.
                    parser_state.non_operating_vertices.back().emplace_back(paren_root);
                }

                // Move forward a token.
                it += 1;
                continue;
            case mflex::TokenType::ASSIGN_TYPE:
                // Add ASSIGN_TYPE vertex.
                add_operating_node(
                    it,
                    mfast::AssignTypeOperatorNode{ it, it },
                    ast,
                    nodes,
                    parser_state
                );
                continue;
            case mflex::TokenType::ASSIGN_VALUE:
                // Add ASSIGN_VALUE vertex.
                add_operating_node(
                    it,
                    mfast::AssignValueOperatorNode{ it, it },
                    ast,
                    nodes,
                    parser_state
                );
                continue;
            case mflex::TokenType::AND:
                // Add AND vertex.
                add_operating_node(
                    it, mfast::AndOperatorNode{ it, it }, ast, nodes, parser_state
                );
                continue;
            case mflex::TokenType::OR:
                // Add OR vertex.
                add_operating_node(
                    it, mfast::OrOperatorNode{ it, it }, ast, nodes, parser_state
                );
                continue;
            case mflex::TokenType::EQUALS:
                // Add EQUALS vertex.
                add_operating_node(
                    it, mfast::EqualOperatorNode{ it, it }, ast, nodes, parser_state
                );
                continue;
            case mflex::TokenType::NOT_EQUALS:
                // Add NOT EQUALS vertex.
                add_operating_node(
                    it, mfast::NotEqualOperatorNode{ it, it }, ast, nodes, parser_state
                );
                continue;
            case mflex::TokenType::LESS_THAN:
                // Add LESS THAN vertex.
                add_operating_node(
                    it, mfast::LesserOperatorNode{ it, it }, ast, nodes, parser_state
                );
                continue;
            case mflex::TokenType::LESS_THAN_OR_EQUAL_TO:
                // Add LESS THAN OR EQUAL TO vertex.
                add_operating_node(
                    it,
                    mfast::LesserOrEqualOperatorNode{ it, it },
                    ast,
                    nodes,
                    parser_state
                );
                continue;
            case mflex::TokenType::GREATER_THAN:
                // Add GREATER THAN vertex.
                add_operating_node(
                    it, mfast::GreaterOperatorNode{ it, it }, ast, nodes, parser_state
                );
                continue;
            case mflex::TokenType::GREATER_THAN_OR_EQUAL_TO:
                // Add GREATER THAN OR EQUAL TO vertex.
                add_operating_node(
                    it,
                    mfast::GreaterOrEqualOperatorNode{ it, it },
                    ast,
                    nodes,
                    parser_state
                );
                continue;
            case mflex::TokenType::PLUS:
                // Add ADDITION vertex.
                add_operating_node(
                    it, mfast::AdditionOperatorNode{ it, it }, ast, nodes, parser_state
                );
                continue;
            case mflex::TokenType::MINUS:
                // TODO(Matthew): for now this token can be both repr of unary and
                //                binary operations (subtraction and negation).
                // Add SUBTRACTION vertex.
                add_operating_node(
                    it,
                    mfast::SubtractionOperatorNode{ it, it },
                    ast,
                    nodes,
                    parser_state
                );
                continue;
            case mflex::TokenType::SLASH:
                // Add DIVIDE vertex.
                add_operating_node(
                    it, mfast::DivisionOperatorNode{ it, it }, ast, nodes, parser_state
                );
                continue;
            case mflex::TokenType::STAR:
                // Add MULTIPLY vertex.
                add_operating_node(
                    it,
                    mfast::MultiplicationOperatorNode{ it, it },
                    ast,
                    nodes,
                    parser_state
                );
                continue;
            case mflex::TokenType::POWER:
                // Add POWER vertex.
                add_operating_node(
                    it, mfast::PowerOperatorNode{ it, it }, ast, nodes, parser_state
                );
                continue;
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
            case mflex::TokenType::IDENTIFIER:
                // Add identifier vertex.
                add_non_operating_node(
                    it,
                    mfast::IdentifierNode{ it, it, it->identifier_idx },
                    ast,
                    nodes,
                    parser_state
                );
                continue;
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

    link_operations_on_stack(Precedence::NONE, ast, nodes, parser_state);

    // If we get here and cursor is not pointing to top-level block, then something has
    // gone wrong in parsing.
    // assert(parser_state.vertices.size() == 0);
}
