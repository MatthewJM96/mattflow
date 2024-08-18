#ifndef __mattflow_ast_node_number_h
#define __mattflow_ast_node_number_h

#include "ast/node/node.h"
#include "literal/number.h"

namespace mattflow {
    namespace ast {
        /**
         * @brief Node reflecting a number literal.
         *
         * @inedge operator | paren expression | assignment | block expression
         */
        struct NumberNode : public Node {
            mflit::Number value;
        };

        /**
         * @brief Node reflecting a string literal.
         *
         * @inedge operator | paren expression | assignment | block expression
         */
        struct StringNode : public Node {
            std::string value;
        };

        /**
         * @brief Node reflecting an identifier literal.
         *
         * @inedge block expression
         * @outedge type expression
         * @outedge rvalue expression
         */
        struct IdentifierNode : public Node {
            mflit::IdentifierIdx name;
        };
    }  // namespace ast
}  // namespace mattflow
namespace mfast = mattflow::ast;

#endif  // __mattflow_ast_node_number_h
