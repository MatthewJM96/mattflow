#ifndef __mattflow_ast_node_operator_h
#define __mattflow_ast_node_operator_h

#include "ast/node/node.h"
#include "lex/token_type.h"

namespace mattflow {
    namespace ast {
        /**
         * @brief Node reflecting a Not operator.
         *
         * @inedge operator | paren expression | assignment | block expression
         * @outedge rvalue expression
         */
        struct NotOperatorNode : public Node {
            // Empty.
        };

        /**
         * @brief Node reflecting a Negation operator.
         *
         * @inedge operator | paren expression | assignment | block expression
         * @outedge rvalue expression
         */
        struct NegationOperatorNode : public Node {
            // Empty.
        };

        /**
         * @brief Node reflecting a Multiplication operator.
         *
         * @inedge operator | paren expression | assignment | block expression
         * @outedge rvalue expression
         * @outedge rvalue expression
         */
        struct MultiplicationOperatorNode : public Node {
            // Empty.
        };

        /**
         * @brief Node reflecting a Division operator.
         *
         * @inedge operator | paren expression | assignment | block expression
         * @outedge rvalue expression
         * @outedge rvalue expression
         */
        struct DivisionOperatorNode : public Node {
            // Empty.
        };

        /**
         * @brief Node reflecting an Addition operator.
         *
         * @inedge operator | paren expression | assignment | block expression
         * @outedge rvalue expression
         * @outedge rvalue expression
         */
        struct AdditionOperatorNode : public Node {
            // Empty.
        };

        /**
         * @brief Node reflecting a Subtraction operator.
         *
         * @inedge operator | paren expression | assignment | block expression
         * @outedge rvalue expression
         * @outedge rvalue expression
         */
        struct SubtractionOperatorNode : public Node {
            // Empty.
        };

        /**
         * @brief Node reflecting a Greater operator.
         *
         * @inedge operator | paren expression | assignment | block expression
         * @outedge rvalue expression
         * @outedge rvalue expression
         */
        struct GreaterOperatorNode : public Node {
            // Empty.
        };

        /**
         * @brief Node reflecting a GreaterOrEqual operator.
         *
         * @inedge operator | paren expression | assignment | block expression
         * @outedge rvalue expression
         * @outedge rvalue expression
         */
        struct GreaterOrEqualOperatorNode : public Node {
            // Empty.
        };

        /**
         * @brief Node reflecting a Lesser operator.
         *
         * @inedge operator | paren expression | assignment | block expression
         * @outedge rvalue expression
         * @outedge rvalue expression
         */
        struct LesserOperatorNode : public Node {
            // Empty.
        };

        /**
         * @brief Node reflecting a LesserOrEqual operator.
         *
         * @inedge operator | paren expression | assignment | block expression
         * @outedge rvalue expression
         * @outedge rvalue expression
         */
        struct LesserOrEqualOperatorNode : public Node {
            // Empty.
        };

        /**
         * @brief Node reflecting an Equal operator.
         *
         * @inedge operator | paren expression | assignment | block expression
         * @outedge rvalue expression
         * @outedge rvalue expression
         */
        struct EqualOperatorNode : public Node {
            // Empty.
        };

        /**
         * @brief Node reflecting a NotEqual operator.
         *
         * @inedge operator | paren expression | assignment | block expression
         * @outedge rvalue expression
         * @outedge rvalue expression
         */
        struct NotEqualOperatorNode : public Node {
            // Empty.
        };

        /**
         * @brief Node reflecting an Or operator.
         *
         * @inedge operator | paren expression | assignment | block expression
         * @outedge rvalue expression
         * @outedge rvalue expression
         */
        struct OrOperatorNode : public Node {
            // Empty.
        };

        /**
         * @brief Node reflecting an And operator.
         *
         * @inedge operator | paren expression | assignment | block expression
         * @outedge rvalue expression
         * @outedge rvalue expression
         */
        struct AndOperatorNode : public Node {
            // Empty.
        };

        /**
         * @brief Node reflecting a value assignment operator.
         *
         * @inedge lvalue expression
         * @outedge rvalue expression
         */
        struct AssignmentOperatorNode : public Node {
            // Empty.
        };
    }  // namespace ast
}  // namespace mattflow
namespace mfast = mattflow::ast;

#endif  // __mattflow_ast_node_operator_h
