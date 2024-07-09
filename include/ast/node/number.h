#ifndef __mattflow_ast_node_number_h
#define __mattflow_ast_node_number_h

#include "ast/node/node.h"
#include "number.h"

namespace mattflow {
    namespace ast {
        struct NumberNode : public Node {
            Number value;
        };
    }  // namespace ast
}  // namespace mattflow
namespace mfast = mattflow::ast;

#endif  // __mattflow_ast_node_number_h
