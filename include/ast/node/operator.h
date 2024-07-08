#ifndef __mattflow_ast_node_operator_h
#define __mattflow_ast_node_operator_h

#include "ast/node/node.h"

namespace mattflow {
    namespace ast {
        struct UnaryOperatorNode : public Node { };

        struct BinaryOperatorNode : public Node { };
    }  // namespace ast
}  // namespace mattflow
namespace mfast = mattflow::ast;

#endif  // __mattflow_ast_node_operator_h
