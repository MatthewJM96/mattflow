#include "stdafx.h"

#include "ast/operator.h"
#include "ast/state.h"

#include "ast/link.h"

void mfast::maybe_link_operations_on_stack(
    VALOUT mfast::AST& ast,
    VALOUT mfast::NodeBuffers& nodes,
    VALOUT mfast::ParserState& parser_state
) {
    // If we have two non-operating nodes in a row, we have a break of expression.
    // TODO(Matthew): handle other control flow
    if (parser_state.last_seen.back() == mfast::NodeCategory::NONOP) {
        // We detect the end of an if expression by the fact we have two consecutive
        // expressions not separated by a keyword.
        if ((parser_state.enclosed_by.back() & EnclosingProps::IF)
            == EnclosingProps::IF)
        {
            mfassert(
                parser_state.last_seen.back() == NodeCategory::THEN
                    || parser_state.last_seen.back() == NodeCategory::ELSE,
                "Closing an if-expression whose last block is neither a then or else "
                "expression."
            );

            // Pop if enclosure.
            pop_enclosure(EnclosingProps::IF, ast, nodes, parser_state);
        } else if ((parser_state.enclosed_by.back() & EnclosingProps::FOR) == EnclosingProps::FOR)
        {
            mfassert(
                parser_state.last_seen.back() == NodeCategory::DO,
                "Closing a for-expression whose last block is not a do expression."
            );

            // Pop for enclosure.
            pop_enclosure(EnclosingProps::FOR, ast, nodes, parser_state);
        }

        mfast::link_operations_on_stack(
            mfast::Precedence::NONE, ast, nodes, parser_state
        );
    }
}

void mfast::link_operations_on_stack(
    VALIN mfast::Precedence target_precedence,
    VALOUT mfast::AST& ast,
    VALOUT mfast::NodeBuffers& nodes,
    VALOUT mfast::ParserState& parser_state
) {
    const size_t NONE_VERT             = std::numeric_limits<size_t>::max();
    const size_t STITCH_TO_NEXT_NON_OP = std::numeric_limits<size_t>::max();

    mfast::Order         op_order;
    mfast::Precedence    op_precedence;
    mfast::Associativity op_associativity;

    auto& op_verts    = parser_state.operating_vertices.back();
    auto& nonop_verts = parser_state.non_operating_vertices.back();

    // Done if nothing to link.
    if (op_verts.size() == 0) return;

    // Current operator to link up.
    size_t           prev_op_vert = NONE_VERT;
    size_t           op_vert      = NONE_VERT;
    mfast::NodeInfo* op           = nullptr;

    size_t stitch_to = STITCH_TO_NEXT_NON_OP;

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
        if (op_precedence <= target_precedence && prev_op_vert != NONE_VERT) {
            // Make sure to add the stitch target for next link phase.
            nonop_verts.emplace_back(prev_op_vert);
            return;
        }

        if (op_order == mfast::Order::UNARY) {
            // Unary operator.

            // Any unary operator should be right-associative.
            mfassert(
                op_associativity == mfast::Associativity::RIGHT,
                "Encountered left-associative unary operator: %s",
                std::visit(mfast::GetReprVisitor{}, *op)
            );

            // If we don't have an operator to stitch to, then stitch to last
            // non-operating vertex. If we do, then stitch to that operator.
            if (stitch_to == STITCH_TO_NEXT_NON_OP) {
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

                // Pop the current operator and consumed non-operators.
                op_verts.pop_back();
            } else {
                // Binary left-associative opeartor.

                // Hard case, here we have to traverse the set of operators with the
                // same precedence and then stitch them in starting with the last of
                // them working to the current operator we are dealing with.

                [[maybe_unused]] mfast::Order         next_op_order;
                mfast::Precedence                     next_op_precedence;
                [[maybe_unused]] mfast::Associativity next_op_associativity;

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
                        mfassert(
                            next_op_order == mfast::Order::BINARY,
                            "Encountered a same-precedence operator with different "
                            "order: %s",
                            std::visit(mfast::GetReprVisitor{}, *op)
                        );
                        mfassert(
                            next_op_associativity == op_associativity,
                            "Encountered a same-precedence operator with different "
                            "associativity: %s",
                            std::visit(mfast::GetReprVisitor{}, *op)
                        );

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
                if (stitch_to != STITCH_TO_NEXT_NON_OP) {
                    --num_non_ops;
                }

                // Stitch to for next operator. This should be == to stitch_to for the
                // operator at the top of the stack.
                size_t non_op_idx_offset = num_non_ops;
                size_t stitch_next_to    = STITCH_TO_NEXT_NON_OP;

                // We have the level of operator to go down to, stitch starting with the
                // one lowest in the stack and work back up.
                for (; next_op_idx < op_verts.size(); ++next_op_idx) {
                    // Update vertex for next operator to point to next lowest operator
                    // we need to stitch.
                    next_op_vert = op_verts[next_op_idx];

                    // If stitch_next_to is set, then the LHS points to that operator,
                    // otherwise it points to the appropriate non-operator.
                    size_t lhs_stitch;
                    if (stitch_next_to == STITCH_TO_NEXT_NON_OP) {
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
                        if (stitch_to == STITCH_TO_NEXT_NON_OP) {
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

    // Make sure to put whatever operator we've just consumed onto the non-op stack.
    nonop_verts.emplace_back(op_vert);
}
