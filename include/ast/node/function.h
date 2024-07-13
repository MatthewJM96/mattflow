#ifndef __mattflow_ast_node_function_h
#define __mattflow_ast_node_function_h

#include "ast/node/node.h"
#include "literal/identifier.h"
#include "type.h"

namespace mattflow {
    namespace ast {
        struct FunctionNode : public Node {
            mftype::Type                      type;
            std::vector<mflit::IdentifierIdx> argument_names;
        };
    }  // namespace ast
}  // namespace mattflow
namespace mfast = mattflow::ast;

#endif  // __mattflow_ast_node_function_h
