#ifndef __mattflow_ast_node_operator_h
#define __mattflow_ast_node_operator_h

#include "ast/node/node.h"

namespace mattflow {
    namespace ast {
        struct UnaryOperatorNode : public Node {
            UnaryOperatorNode();

            MATTFLOW_NON_COPYABLE(UnaryOperatorNode);
        };

        struct BinaryOperatorNode : public Node {
            BinaryOperatorNode();

            MATTFLOW_NON_COPYABLE(BinaryOperatorNode);
        };
    }  // namespace ast
}  // namespace mattflow
namespace mfast = mattflow::ast;

#endif  // __mattflow_ast_node_operator_h
