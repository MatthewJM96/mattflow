#include "stdafx.h"

#include "ast/node.h"
#include "ast/operator.h"
#include "literal/identifier.h"

#include "ast/parse.h"

static void link_operations_on_stack(
    VALIN mfast::Precedence target_precedence,
    VALOUT mfast::AST& ast,
    VALOUT mfast::NodeBuffers& nodes,
    VALOUT mfast::ParserState& parser_state
) {
    mfast::GetOrderVisitor         op_order;
    mfast::GetPrecedenceVisitor    op_precedence;
    mfast::GetAssociativityVisitor op_associativity;

    // Current operator to link up.
    auto  op_vert = parser_state.operating_vertices.back().back();
    auto* op      = &nodes.get_node_info(op_vert);

    const auto STITCH_TO_NEXT_NON_OP = std::numeric_limits<decltype(op_vert)>::max();
    auto       stitch_to             = STITCH_TO_NEXT_NON_OP;

    while (parser_state.operating_vertices.size() != 0) {
        // Get order, precedence, and associativity of operator.
        std::visit(op_order, *op);
        std::visit(op_precedence, *op);
        std::visit(op_associativity, *op);

        // We are done stitching if the operator we are about to consider has the
        // target precedence.
        if (op_precedence.result == target_precedence) return;

        if (op_order.result == mfast::Order::UNARY) {
            // Unary operator.

            // Any unary operator should be right-associative.
            assert(op_associativity.result == mfast::Associativity::RIGHT);

            // If we don't have an operator to stitch to, then stitch to last
            // non-operating vertex. If we do, then stitch to that operator.
            if (stitch_to == STITCH_TO_NEXT_NON_OP) {
                boost::add_edge(
                    op_vert, parser_state.non_operating_vertices.back().back(), ast
                );
                parser_state.non_operating_vertices.back().pop_back();
            } else {
                boost::add_edge(op_vert, stitch_to, ast);
            }

            // We should now stitch to this operator we have just dealt with.
            stitch_to = op_vert;

            // Pop the current operator.
            parser_state.operating_vertices.back().pop_back();
        } else {
            // Binary operator.

            if (op_associativity.result == mfast::Associativity::RIGHT) {
                // Binary right-associative opeartor.

                // Simple case. We can just deal with this operator without worrying
                // about subsequent operators.

                // If we don't have an operator to stitch to, then stitch to last
                // non-operating vertex. If we do, then stitch to that operator.
                if (stitch_to == STITCH_TO_NEXT_NON_OP) {
                    // Stitch to last two non-operating vertices.
                    boost::add_edge(
                        op_vert,
                        *(parser_state.non_operating_vertices.back().end() - 2),
                        ast
                    );
                    boost::add_edge(
                        op_vert,
                        *(parser_state.non_operating_vertices.back().end() - 1),
                        ast
                    );

                    // Pop those non-operating vertices.
                    parser_state.non_operating_vertices.back().pop_back();
                    parser_state.non_operating_vertices.back().pop_back();
                } else {
                    // Stitch to last non-operating vertex, and the stitch_to target.
                    boost::add_edge(
                        op_vert, parser_state.non_operating_vertices.back().back(), ast
                    );
                    boost::add_edge(op_vert, stitch_to, ast);

                    // Pop the non-operating vertex.
                    parser_state.non_operating_vertices.back().pop_back();
                }

                // We should now stitch to this operator we have just dealt with.
                stitch_to = op_vert;

                // Pop the current operator.
                parser_state.operating_vertices.back().pop_back();
            } else {
                // Binary left-associative opeartor.

                // Hard case, here we have to traverse the set of operators with the
                // same precedence and then stitch them in starting with the last of
                // them working to the current operator we are dealing with.

                // TODO(Matthew) loop down stack of operators til precedence changes -
                //               by necessity associativity of all should be the same
                //               (but maybe assert this) - and then work from the
                //               bottom-most back up to the top one and pop all
                //               subsequently.

                mfast::GetOrderVisitor         next_op_order;
                mfast::GetPrecedenceVisitor    next_op_precedence;
                mfast::GetAssociativityVisitor next_op_associativity;

                // Look at operator subsequent to the top op of the stack.
                size_t next_op_idx = parser_state.operating_vertices.back().size() - 2;
                auto next_op_vert = parser_state.operating_vertices.back()[next_op_idx];
                auto* next_op     = &nodes.get_node_info(next_op_vert);

                // Get order, precedence, and associativity of the next operator.
                std::visit(next_op_order, *next_op);
                std::visit(next_op_precedence, *next_op);
                std::visit(next_op_associativity, *next_op);

                // Consider each operator in turn walking down the stack, if the
                // precedence is still the same, it gets included in the operators that
                // will be stitched.
                while (next_op_precedence.result == op_precedence.result) {
                    // Operators of same precedence as the first we have considered here
                    // MUST also be binary operators of the same associativity.
                    assert(next_op_order.result == mfast::Order::BINARY);
                    assert(next_op_associativity.result == op_associativity.result);

                    // If we have just considered the last operator on the stack, then
                    // we are done and need to stitch everything.
                    if (next_op_idx == 0) break;

                    // Look at the next operator on the stack.
                    --next_op_idx;
                    next_op_vert = parser_state.operating_vertices.back()[next_op_idx];
                    next_op      = &nodes.get_node_info(next_op_vert);

                    // Get order, precedence, and associativity of that operator.
                    std::visit(next_op_order, *op);
                    std::visit(next_op_precedence, *op);
                    std::visit(next_op_associativity, *op);
                }

                // next_op_idx is pointing to the operator after where we intend to
                // stitch to.
                ++next_op_idx;
                // Number of ops we'll be stitching. NOTE: as we are stitching binary
                // operators we know for sure that the number of non-operator nodes
                // involved in the stitching will be one greater than the number of
                // operators.
                size_t num_ops
                    = parser_state.operating_vertices.back().size() - next_op_idx;
                size_t num_non_ops = num_ops + 1;

                // Stitch to for next operator. This should be == to stitch_to for the
                // operator at the top of the stack.
                size_t non_op_idx_offset = num_ops + 1;
                auto   stitch_next_to    = STITCH_TO_NEXT_NON_OP;

                // We have the level of operator to go down to, stitch starting with the
                // one lowest in the stack and work back up.
                for (; next_op_idx <= parser_state.operating_vertices.back().size() - 1;
                     ++next_op_idx)
                {
                    // Update vertex for next operator to point to next lowest operator
                    // we need to stitch.
                    next_op_vert = parser_state.operating_vertices.back()[next_op_idx];

                    // If we don't have an operator to stitch to, then stitch to last
                    // non-operating vertex. If we do, then stitch to that operator.
                    if (stitch_next_to == STITCH_TO_NEXT_NON_OP) {
                        // Stitch to last two non-operating vertices.
                        boost::add_edge(
                            next_op_vert,
                            *(parser_state.non_operating_vertices.back().end()
                              - non_op_idx_offset--),
                            ast
                        );
                        boost::add_edge(
                            next_op_vert,
                            *(parser_state.non_operating_vertices.back().end()
                              - non_op_idx_offset--),
                            ast
                        );
                    } else {
                        // Stitch the stitch_to target and then the last non-operating
                        // vertex.
                        boost::add_edge(next_op_vert, stitch_next_to, ast);
                        boost::add_edge(
                            next_op_vert,
                            *(parser_state.non_operating_vertices.back().end()
                              - non_op_idx_offset--),
                            ast
                        );
                    }

                    // We should now stitch to this operator we have just dealt with.
                    stitch_next_to = next_op_vert;
                }

                // Set stitch_to to the last of the operators.
                stitch_to = parser_state.operating_vertices.back().back();

                // Erase the operators and non-operators we have stitched.
                parser_state.operating_vertices.back().erase(
                    parser_state.operating_vertices.back().end() - num_ops,
                    parser_state.operating_vertices.back().end()
                );
                parser_state.non_operating_vertices.back().erase(
                    parser_state.non_operating_vertices.back().end() - num_non_ops,
                    parser_state.non_operating_vertices.back().end()
                );
            }
        }

        // TODO(Matthew): probably add the stitch_to vertex back onto the
        //                operating_vertices stack?

        // Get next operator to link up.
        op_vert = parser_state.operating_vertices.back().back();
        op      = &nodes.get_node_info(op_vert);
    }
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
    if (parser_state.last_seen.back() != mfast::NodeCategory::NONE
        && parser_state.last_seen.back() != mfast::NodeCategory::BINOP
        && parser_state.last_seen.back() != mfast::NodeCategory::UNOP)
    {
        link_operations_on_stack(mfast::Precedence::NONE, ast, nodes, parser_state);

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
        link_operations_on_stack(node_info.PRECEDENCE, ast, nodes, parser_state);
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
