#ifndef __mattflow_ast_node_control_flow_h
#define __mattflow_ast_node_control_flow_h

#include "ast/node/node.h"
#include "literal/identifier.h"
#include "type.h"

namespace mattflow {
    namespace ast {
        /**
         * @brief Node reflecting an if block. Each IfNode will have pairs of out edges,
         * one an assignment, the other an expression for each if/elif-then block, and
         * at most one more outedge to an expression for a final else.
         *
         * @inedge operator | paren expression | assignment | block expression
         * @outedges rvalue assignment + expression
         * @outedge expression
         */
        struct IfNode : public Node {
            // Empty.
        };

        /**
         * @brief Node reflecting a for block.
         *
         * @inedge operator | paren expression | assignment | block expression
         * @outedges range
         * @outedge expression
         */
        struct ForNode : public Node {
            // Empty.
        };

        /**
         * @brief Node reflecting a range. The first outedge is an identifier the range
         * is assigned to, the second outedge is to an expression providing a valid
         * range to iterate over, and the third optional outedge is the filtering
         * "where" logic.
         *
         * @inedge operator | paren expression | assignment | block expression
         * @outedge identifier
         * @outedge expression
         * @outedge logic
         */
        struct RangeNode : public Node {
            // Empty.
        };

        /**
         * @brief Node reflecting a while block. The first outedge is an expression
         * returning a Boolean, and the second outedge is an expression evaluated so
         * long as evaluation of the first returns true.
         *
         * @inedge operator | paren expression | assignment | block expression
         * @outedge expression
         * @outedge expression
         */
        struct WhileNode : public Node {
            // Empty.
        };
    }  // namespace ast
}  // namespace mattflow
namespace mfast = mattflow::ast;

#endif  // __mattflow_ast_node_control_flow_h
