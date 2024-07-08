#ifndef __mattflow_ast_node_function_h
#define __mattflow_ast_node_function_h

#include "ast/node/node.h"

namespace mattflow {
    namespace ast {
        struct PrototypeNode : public Node {
            PrototypeNode();

            MATTFLOW_NON_COPYABLE(PrototypeNode);
        };

        class FunctionNode : public Node {
            FunctionNode();

            MATTFLOW_NON_COPYABLE(FunctionNode);
        };
    }  // namespace ast
}  // namespace mattflow
namespace mfast = mattflow::ast;

#endif  // __mattflow_ast_node_function_h
