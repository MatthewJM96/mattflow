#ifndef __mattflow_ast_node_function_h
#define __mattflow_ast_node_function_h

#include "../../type.h"
#include "ast/node/node.h"
#include "literal/identifier.h"

namespace mattflow {
    namespace ast {
        /**
         * @brief Node reflecting a function definition.
         *
         * @inedge operator | paren expression | assignment | block expression
         * @outedge rvalue expression
         */
        struct FunctionNode : public Node {
            mftype::Type*                     type;
            std::vector<mflit::IdentifierIdx> argument_names;
        };
    }  // namespace ast
}  // namespace mattflow
namespace mfast = mattflow::ast;

#endif  // __mattflow_ast_node_function_h
