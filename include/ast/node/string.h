#ifndef __mattflow_ast_node_string_h
#define __mattflow_ast_node_string_h

#include "ast/node/node.h"

namespace mattflow {
    namespace ast {
        /**
         * @brief Node reflecting a string literal.
         *
         * @inedge operator | paren expression | assignment | block expression
         */
        struct StringNode : public Node {
            std::string value;
        };
    }  // namespace ast
}  // namespace mattflow
namespace mfast = mattflow::ast;

#endif  // __mattflow_ast_node_string_h
