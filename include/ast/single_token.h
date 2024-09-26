#ifndef __mattflow_ast_single_token_h
#define __mattflow_ast_single_token_h

#include "ast/ast.h"
#include "ast/node.h"
#include "lex/token.h"

namespace mattflow {
    namespace ast {
        template <NodeType _NodeType, typename... Args>
        void add_single_token_nonop(
            VALINOUT mflex::Tokens::const_iterator& curr_token,
            VALOUT mfast::AST& ast,
            VALOUT mfast::NodeBuffers& nodes,
            VALOUT mfast::ParserState& parser_state,
            Args&&... args
        ) {
            // Link operations on stack if we have an end of expression.
            mfast::maybe_link_operations_on_stack(ast, nodes, parser_state);

            // Add vertex to AST for node, and push it onto the stack.
            auto vertex = boost::add_vertex(ast);
            parser_state.non_operating_vertices.back().emplace_back(vertex);

            // Add node info about bool node and associate with vertex in AST.
            nodes.vertex_node_map[vertex] = nodes.node_info.size();
            nodes.node_info.emplace_back(_NodeType{
                curr_token, curr_token, std::forward<Args>(args)... });

            parser_state.last_seen.back() = mfast::NodeCategory::NONOP;

            // Move forward a token.
            curr_token += 1;
        }

        template <NodeType _NodeType, typename... Args>
        void add_single_token_op(
            VALINOUT mflex::Tokens::const_iterator& curr_token,
            VALOUT mfast::AST& ast,
            VALOUT mfast::NodeBuffers& nodes,
            VALOUT mfast::ParserState& parser_state,
            Args&&... args
        ) {
            _NodeType node_info
                = _NodeType{ curr_token, curr_token, std::forward<Args>(args)... };

#if DEBUG
            // If precedence is the same, associativity must be too.
            if (node_info.PRECEDENCE == parser_state.precedence.back()) {
                mfassert(
                    node_info.ASSOCIATIVITY == parser_state.associativity.back(),
                    "Trying to add same-precedence different-associativity operator as "
                    "previous: %s",
                    node_info.debug_repr()
                );
            }
#endif  // DEBUG

            // If operator has lower precedence than the highest we've so far
            // encountered, we need to deal with the expression as formed so far (up to
            // any preceding operation with the same precedence as this to-be-added
            // operation) before continuing on. In either case the current block/paren
            // expr precedence must be overwritten with this operator's precedence.
            if (node_info.PRECEDENCE < parser_state.precedence.back()) {
                mfast::link_operations_on_stack(
                    node_info.PRECEDENCE, ast, nodes, parser_state
                );
            }
            parser_state.precedence.back()    = node_info.PRECEDENCE;
            parser_state.associativity.back() = node_info.ASSOCIATIVITY;

            // Add vertex to AST for node, and push it onto the stack.
            auto vertex = boost::add_vertex(ast);
            parser_state.operating_vertices.back().emplace_back(vertex);

            // Add node info about bool node and associate with vertex in AST.
            nodes.vertex_node_map[vertex] = nodes.node_info.size();
            nodes.node_info.emplace_back(node_info);

            if (node_info.ORDER == mfast::Order::UNARY) {
                parser_state.last_seen.back() = mfast::NodeCategory::UNOP;
            } else {
                parser_state.last_seen.back() = mfast::NodeCategory::BINOP;
            }

            // Move forward a token.
            curr_token += 1;
        }
    }  // namespace ast
}  // namespace mattflow
namespace mfast = mattflow::ast;

#endif  // __mattflow_ast_single_token_h
