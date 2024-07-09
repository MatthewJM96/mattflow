#ifndef __mattflow_ast_node_variable_h
#define __mattflow_ast_node_variable_h

#include "ast/node/node.h"
#include "type.h"

namespace mattflow {
    namespace ast {
        struct VariableNode : public Node {
            mftype::Type         type;
            mflit::IdentifierIdx identifier_idx;
        };
    }  // namespace ast
}  // namespace mattflow
namespace mfast = mattflow::ast;

#endif  // __mattflow_ast_node_variable_h
