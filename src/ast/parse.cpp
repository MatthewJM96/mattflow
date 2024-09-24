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
    mfast::Order         op_order;
    mfast::Precedence    op_precedence;
    mfast::Associativity op_associativity;

    auto& op_verts    = parser_state.operating_vertices.back();
    auto& nonop_verts = parser_state.non_operating_vertices.back();

    // Done if nothing to link.
    if (op_verts.size() == 0) return;

    // Current operator to link up.
    size_t           prev_op_vert;
    size_t           op_vert;
    mfast::NodeInfo* op;

    size_t stitch_to = mfast::STITCH_TO_NEXT_NON_OP;

    while (op_verts.size() != 0) {
        // Get next operator to link up.
        prev_op_vert = op_vert;
        op_vert      = op_verts.back();
        op           = &nodes.get_node_info(op_vert);

        // Get order, precedence, and associativity of operator.
        op_order         = std::visit(mfast::GetOrderVisitor{}, *op);
        op_precedence    = std::visit(mfast::GetPrecedenceVisitor{}, *op);
        op_associativity = std::visit(mfast::GetAssociativityVisitor{}, *op);

        // We are done stitching if the operator we are about to consider has the
        // target precedence.
        if (op_precedence == target_precedence) {
            // Make sure to add the stitch target for next link phase.
            nonop_verts.emplace_back(prev_op_vert);
            return;
        }

        if (op_order == mfast::Order::UNARY) {
            // Unary operator.

            // Any unary operator should be right-associative.
            assert(op_associativity == mfast::Associativity::RIGHT);

            // If we don't have an operator to stitch to, then stitch to last
            // non-operating vertex. If we do, then stitch to that operator.
            if (stitch_to == mfast::STITCH_TO_NEXT_NON_OP) {
                boost::add_edge(op_vert, nonop_verts.back(), ast);
                nonop_verts.pop_back();
            } else {
                boost::add_edge(op_vert, stitch_to, ast);
            }

            // We should now stitch to this operator we have just dealt with.
            stitch_to = op_vert;

            // Pop the current operator.
            op_verts.pop_back();
        } else {
            // Binary operator.

            if (op_associativity == mfast::Associativity::RIGHT) {
                // Binary right-associative opeartor.

                // Simple case. We can just deal with this operator without worrying
                // about subsequent operators.

                // If we don't have an operator to stitch to, then stitch to last
                // non-operating vertex. If we do, then stitch to that operator.
                if (stitch_to == mfast::STITCH_TO_NEXT_NON_OP) {
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

                // Pop the current operator and consumed non-operators.
                op_verts.pop_back();
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

                mfast::Order         next_op_order;
                mfast::Precedence    next_op_precedence;
                mfast::Associativity next_op_associativity;

                size_t           next_op_idx;
                size_t           next_op_vert;
                mfast::NodeInfo* next_op;

                if (op_verts.size() > 1) {
                    // Look at operator subsequent to the top op of the stack.
                    next_op_idx  = op_verts.size() - 2;
                    next_op_vert = op_verts[next_op_idx];
                    next_op      = &nodes.get_node_info(next_op_vert);

                    // Get order, precedence, and associativity of the next operator.
                    next_op_order = std::visit(mfast::GetOrderVisitor{}, *next_op);
                    next_op_precedence
                        = std::visit(mfast::GetPrecedenceVisitor{}, *next_op);
                    next_op_associativity
                        = std::visit(mfast::GetAssociativityVisitor{}, *next_op);

                    // Consider each operator in turn walking down the stack, if the
                    // precedence is still the same, it gets included in the operators
                    // that will be stitched.
                    while (next_op_precedence == op_precedence) {
                        // Operators of same precedence as the first we have considered
                        // here MUST also be binary operators of the same associativity.
                        assert(next_op_order == mfast::Order::BINARY);
                        assert(next_op_associativity == op_associativity);

                        // If we have just considered the last operator on the stack,
                        // then we are done and need to stitch everything.
                        if (next_op_idx == 0) {
                            // Decrement as we expect that afterwards.
                            --next_op_idx;
                            break;
                        }

                        // Look at the next operator on the stack.
                        --next_op_idx;
                        next_op_vert = op_verts[next_op_idx];
                        next_op      = &nodes.get_node_info(next_op_vert);

                        // Get order, precedence, and associativity of that operator.
                        next_op_order = std::visit(mfast::GetOrderVisitor{}, *next_op);
                        next_op_precedence
                            = std::visit(mfast::GetPrecedenceVisitor{}, *next_op);
                        next_op_associativity
                            = std::visit(mfast::GetAssociativityVisitor{}, *next_op);
                    }

                    // next_op_idx is pointing to the operator after where we intend to
                    // stitch to.
                    ++next_op_idx;
                } else {
                    next_op_idx = 0;
                }

                // Number of ops we'll be stitching. NOTE: as we are stitching binary
                // operators we know for sure that the number of non-operator nodes
                // involved in the stitching will be one greater than the number of
                // operators.
                size_t num_ops     = op_verts.size() - next_op_idx;
                size_t num_non_ops = num_ops + 1;
                // Deduct one non-op if the RHS is stitching into an operator.
                if (stitch_to != mfast::STITCH_TO_NEXT_NON_OP) {
                    --num_non_ops;
                }

                // Stitch to for next operator. This should be == to stitch_to for the
                // operator at the top of the stack.
                size_t non_op_idx_offset = num_non_ops;
                size_t stitch_next_to    = mfast::STITCH_TO_NEXT_NON_OP;

                // We have the level of operator to go down to, stitch starting with the
                // one lowest in the stack and work back up.
                for (; next_op_idx < op_verts.size(); ++next_op_idx) {
                    // Update vertex for next operator to point to next lowest operator
                    // we need to stitch.
                    next_op_vert = op_verts[next_op_idx];

                    // If stitch_next_to is set, then the LHS points to that operator,
                    // otherwise it points to the appropriate non-operator.
                    size_t lhs_stitch;
                    if (stitch_next_to == mfast::STITCH_TO_NEXT_NON_OP) {
                        lhs_stitch = *(nonop_verts.end() - non_op_idx_offset--);
                    } else {
                        lhs_stitch = stitch_next_to;
                    }

                    // If we are considering the last operator in the sequence of
                    // same-precedence, same-associativity operators, then we stitch
                    // to an operator if stitch_to is set, or else to the appropriate
                    // non-operator.
                    size_t rhs_stitch;
                    if (next_op_idx == op_verts.size() - 1) {
                        if (stitch_to == mfast::STITCH_TO_NEXT_NON_OP) {
                            rhs_stitch = *(nonop_verts.end() - non_op_idx_offset--);
                        } else {
                            rhs_stitch = stitch_to;
                        }
                    } else {
                        rhs_stitch = *(nonop_verts.end() - non_op_idx_offset--);
                    }

                    // Do the stitching.
                    boost::add_edge(next_op_vert, lhs_stitch, ast);
                    boost::add_edge(next_op_vert, rhs_stitch, ast);

                    // We should now stitch to this operator we have just dealt with.
                    stitch_next_to = next_op_vert;
                }

                // Set stitch_to to the last of the operators.
                stitch_to = op_verts.back();

                // Erase the operators and non-operators we have stitched.
                op_verts.erase(op_verts.end() - num_ops, op_verts.end());
                nonop_verts.erase(nonop_verts.end() - num_non_ops, nonop_verts.end());
            }
        }
    }

    nonop_verts.emplace_back(op_vert);
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
            case mflex::TokenType::IDENTIFIER:
            case mflex::TokenType::LEFT_PAREN:
            case mflex::TokenType::RIGHT_PAREN:
            case mflex::TokenType::LEFT_BRACKET:
            case mflex::TokenType::RIGHT_BRACKET:
            case mflex::TokenType::COMMA:
            case mflex::TokenType::RANGE:
            case mflex::TokenType::DOT:
            case mflex::TokenType::ARROW:
            case mflex::TokenType::SENTINEL:
                break;
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

    link_operations_on_stack(Precedence::NONE, ast, nodes, parser_state);

    // If we get here and cursor is not pointing to top-level block, then something has
    // gone wrong in parsing.
    // assert(parser_state.vertices.size() == 0);
}
