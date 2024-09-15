#ifndef __mattflow_ast_node_operator_h
#define __mattflow_ast_node_operator_h

#include "ast/node/node.h"
#include "lex/token_type.h"

namespace mattflow {
    enum class Precedence : int16_t {
        NONE       = 0,
        ASSIGNMENT = 100,
        LOGICAL    = 200,
        EQUALITY   = 300,
        COMPARISON = 400,
        TERM       = 500,
        FACTOR     = 600,
        UNARY      = 700
    };

    enum class Associativity : int16_t {
        SENTINEL,
        NONE,
        LEFT,
        RIGHT
    };

    namespace ast {
        /**
         * @brief Node reflecting a Not operator.
         *
         * @inedge operator | paren expression | assignment | block expression
         * @outedge rvalue expression
         */
        struct NotOperatorNode : public Node {
            const Precedence    PRECEDENCE    = Precedence::UNARY;
            const Associativity ASSOCIATIVITY = Associativity::RIGHT;
        };

        /**
         * @brief Node reflecting a Negation operator.
         *
         * @inedge operator | paren expression | assignment | block expression
         * @outedge rvalue expression
         */
        struct NegationOperatorNode : public Node {
            const Precedence    PRECEDENCE    = Precedence::UNARY;
            const Associativity ASSOCIATIVITY = Associativity::RIGHT;
        };

        /**
         * @brief Node reflecting a Multiplication operator.
         *
         * @inedge operator | paren expression | assignment | block expression
         * @outedge rvalue expression
         * @outedge rvalue expression
         */
        struct MultiplicationOperatorNode : public Node {
            const Precedence    PRECEDENCE    = Precedence::FACTOR;
            const Associativity ASSOCIATIVITY = Associativity::LEFT;
        };

        /**
         * @brief Node reflecting a Division operator.
         *
         * @inedge operator | paren expression | assignment | block expression
         * @outedge rvalue expression
         * @outedge rvalue expression
         */
        struct DivisionOperatorNode : public Node {
            const Precedence    PRECEDENCE    = Precedence::FACTOR;
            const Associativity ASSOCIATIVITY = Associativity::LEFT;
        };

        /**
         * @brief Node reflecting an Addition operator.
         *
         * @inedge operator | paren expression | assignment | block expression
         * @outedge rvalue expression
         * @outedge rvalue expression
         */
        struct AdditionOperatorNode : public Node {
            const Precedence    PRECEDENCE    = Precedence::TERM;
            const Associativity ASSOCIATIVITY = Associativity::LEFT;
        };

        /**
         * @brief Node reflecting a Subtraction operator.
         *
         * @inedge operator | paren expression | assignment | block expression
         * @outedge rvalue expression
         * @outedge rvalue expression
         */
        struct SubtractionOperatorNode : public Node {
            const Precedence    PRECEDENCE    = Precedence::TERM;
            const Associativity ASSOCIATIVITY = Associativity::LEFT;
        };

        /**
         * @brief Node reflecting a Greater operator.
         *
         * @inedge operator | paren expression | assignment | block expression
         * @outedge rvalue expression
         * @outedge rvalue expression
         */
        struct GreaterOperatorNode : public Node {
            const Precedence    PRECEDENCE    = Precedence::COMPARISON;
            const Associativity ASSOCIATIVITY = Associativity::NONE;
        };

        /**
         * @brief Node reflecting a GreaterOrEqual operator.
         *
         * @inedge operator | paren expression | assignment | block expression
         * @outedge rvalue expression
         * @outedge rvalue expression
         */
        struct GreaterOrEqualOperatorNode : public Node {
            const Precedence    PRECEDENCE    = Precedence::COMPARISON;
            const Associativity ASSOCIATIVITY = Associativity::NONE;
        };

        /**
         * @brief Node reflecting a Lesser operator.
         *
         * @inedge operator | paren expression | assignment | block expression
         * @outedge rvalue expression
         * @outedge rvalue expression
         */
        struct LesserOperatorNode : public Node {
            const Precedence    PRECEDENCE    = Precedence::COMPARISON;
            const Associativity ASSOCIATIVITY = Associativity::NONE;
        };

        /**
         * @brief Node reflecting a LesserOrEqual operator.
         *
         * @inedge operator | paren expression | assignment | block expression
         * @outedge rvalue expression
         * @outedge rvalue expression
         */
        struct LesserOrEqualOperatorNode : public Node {
            const Precedence    PRECEDENCE    = Precedence::COMPARISON;
            const Associativity ASSOCIATIVITY = Associativity::NONE;
        };

        /**
         * @brief Node reflecting an Equal operator.
         *
         * @inedge operator | paren expression | assignment | block expression
         * @outedge rvalue expression
         * @outedge rvalue expression
         */
        struct EqualOperatorNode : public Node {
            const Precedence    PRECEDENCE    = Precedence::EQUALITY;
            const Associativity ASSOCIATIVITY = Associativity::LEFT;
        };

        /**
         * @brief Node reflecting a NotEqual operator.
         *
         * @inedge operator | paren expression | assignment | block expression
         * @outedge rvalue expression
         * @outedge rvalue expression
         */
        struct NotEqualOperatorNode : public Node {
            const Precedence    PRECEDENCE    = Precedence::EQUALITY;
            const Associativity ASSOCIATIVITY = Associativity::LEFT;
        };

        /**
         * @brief Node reflecting an Or operator.
         *
         * @inedge operator | paren expression | assignment | block expression
         * @outedge rvalue expression
         * @outedge rvalue expression
         */
        struct OrOperatorNode : public Node {
            const Precedence    PRECEDENCE    = Precedence::LOGICAL;
            const Associativity ASSOCIATIVITY = Associativity::LEFT;
        };

        /**
         * @brief Node reflecting an And operator.
         *
         * @inedge operator | paren expression | assignment | block expression
         * @outedge rvalue expression
         * @outedge rvalue expression
         */
        struct AndOperatorNode : public Node {
            const Precedence    PRECEDENCE    = Precedence::LOGICAL;
            const Associativity ASSOCIATIVITY = Associativity::LEFT;
        };

        /**
         * @brief Node reflecting a value assignment operator.
         *
         * @inedge lvalue expression
         * @outedge rvalue expression
         */
        struct AssignmentOperatorNode : public Node {
            const Precedence    PRECEDENCE    = Precedence::ASSIGNMENT;
            const Associativity ASSOCIATIVITY = Associativity::RIGHT;
        };
    }  // namespace ast
}  // namespace mattflow
namespace mfast = mattflow::ast;

#endif  // __mattflow_ast_node_operator_h
