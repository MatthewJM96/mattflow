#ifndef __mattflow_ast_node_expression_h
#define __mattflow_ast_node_expression_h

#include "ast/node/node.h"
#include "literal/identifier.h"
#include "type.h"

namespace mattflow {
    namespace ast {
        /**
         * @brief Node reflecting a block expression.
         *
         * @inedge TODO
         * @outedges TODO
         * @outedge TODO
         */
        struct BlockExprNode : public Node {
            // Empty.
            BlockExprNode(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) :
                Node(_first_token, _last_token) { }

            BlockExprNode(const BlockExprNode& node) : Node(node) { }

            BlockExprNode(BlockExprNode&& node) : Node(node) { }
        };
    }  // namespace ast
}  // namespace mattflow
namespace mfast = mattflow::ast;

#endif  // __mattflow_ast_node_expression_h
