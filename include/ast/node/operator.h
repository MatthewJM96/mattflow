#ifndef __mattflow_ast_node_operator_h
#define __mattflow_ast_node_operator_h

#include "ast/node/node.h"
#include "lex/token_type.h"

namespace mattflow {
    namespace ast {
        enum class Order {
            UNARY,
            BINARY
        };

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

        enum class Associativity {
            SENTINEL,
            NONE,
            LEFT,
            RIGHT
        };

        /**
         * @brief Node reflecting an operator.
         */
        struct OperatorNode : public Node {
            // Empty.
        };

        /**
         * @brief Node reflecting a Not operator.
         *
         * @inedge operator | paren expression | assignment | block expression
         * @outedge rvalue expression
         */
        struct NotOperatorNode : public OperatorNode {
            const Order         ORDER         = Order::UNARY;
            const Precedence    PRECEDENCE    = Precedence::UNARY;
            const Associativity ASSOCIATIVITY = Associativity::RIGHT;
        };

        /**
         * @brief Node reflecting a Negation operator.
         *
         * @inedge operator | paren expression | assignment | block expression
         * @outedge rvalue expression
         */
        struct NegationOperatorNode : public OperatorNode {
            const Order         ORDER         = Order::UNARY;
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
        struct MultiplicationOperatorNode : public OperatorNode {
            const Order         ORDER         = Order::BINARY;
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
        struct DivisionOperatorNode : public OperatorNode {
            const Order         ORDER         = Order::BINARY;
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
        struct AdditionOperatorNode : public OperatorNode {
            const Order         ORDER         = Order::BINARY;
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
        struct SubtractionOperatorNode : public OperatorNode {
            const Order         ORDER         = Order::BINARY;
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
        struct GreaterOperatorNode : public OperatorNode {
            const Order         ORDER         = Order::BINARY;
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
        struct GreaterOrEqualOperatorNode : public OperatorNode {
            const Order         ORDER         = Order::BINARY;
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
        struct LesserOperatorNode : public OperatorNode {
            const Order         ORDER         = Order::BINARY;
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
        struct LesserOrEqualOperatorNode : public OperatorNode {
            const Order         ORDER         = Order::BINARY;
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
        struct EqualOperatorNode : public OperatorNode {
            const Order         ORDER         = Order::BINARY;
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
        struct NotEqualOperatorNode : public OperatorNode {
            const Order         ORDER         = Order::BINARY;
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
        struct OrOperatorNode : public OperatorNode {
            const Order         ORDER         = Order::BINARY;
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
        struct AndOperatorNode : public OperatorNode {
            const Order         ORDER         = Order::BINARY;
            const Precedence    PRECEDENCE    = Precedence::LOGICAL;
            const Associativity ASSOCIATIVITY = Associativity::LEFT;
        };

        /**
         * @brief Node reflecting a value assignment operator.
         *
         * @inedge lvalue expression
         * @outedge rvalue expression
         */
        struct AssignmentOperatorNode : public OperatorNode {
            const Order         ORDER         = Order::BINARY;
            const Precedence    PRECEDENCE    = Precedence::ASSIGNMENT;
            const Associativity ASSOCIATIVITY = Associativity::RIGHT;
        };
    }  // namespace ast
}  // namespace mattflow
namespace mfast = mattflow::ast;

#endif  // __mattflow_ast_node_operator_h
