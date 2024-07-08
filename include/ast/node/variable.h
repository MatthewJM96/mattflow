#ifndef __mattflow_ast_node_variable_h
#define __mattflow_ast_node_variable_h

#include "ast/node/node.h"

namespace mattflow {
    namespace ast {
        struct VariableNode : public Node {
            VariableNode();

            MATTFLOW_NON_COPYABLE(VariableNode);
        };
    }  // namespace ast
}  // namespace mattflow
namespace mfast = mattflow::ast;

#endif  // __mattflow_ast_node_variable_h
