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
            POWER      = 700,
            UNARY      = 800
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
            OperatorNode(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) :
                Node(_first_token, _last_token) {
                // Empty.
            }
        };

        /**
         * @brief Node reflecting a Not operator.
         *
         * @inedge operator | paren expression | assignment | block expression
         * @outedge rvalue expression
         */
        struct NotOperatorNode : public OperatorNode {
            NotOperatorNode(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) :
                OperatorNode(_first_token, _last_token) {
                // Empty.
            }

            const Order         ORDER         = Order::UNARY;
            const Precedence    PRECEDENCE    = Precedence::UNARY;
            const Associativity ASSOCIATIVITY = Associativity::RIGHT;

            std::string debug_repr() override { return "op !"; }
        };

        /**
         * @brief Node reflecting a Negation operator.
         *
         * @inedge operator | paren expression | assignment | block expression
         * @outedge rvalue expression
         */
        struct NegationOperatorNode : public OperatorNode {
            NegationOperatorNode(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) :
                OperatorNode(_first_token, _last_token) {
                // Empty.
            }

            const Order         ORDER         = Order::UNARY;
            const Precedence    PRECEDENCE    = Precedence::UNARY;
            const Associativity ASSOCIATIVITY = Associativity::RIGHT;

            std::string debug_repr() override { return "op -"; }
        };

        /**
         * @brief Node reflecting a Power operator.
         *
         * @inedge operator | paren expression | assignment | block expression
         * @outedge rvalue expression
         * @outedge rvalue expression
         */
        struct PowerOperatorNode : public OperatorNode {
            PowerOperatorNode(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) :
                OperatorNode(_first_token, _last_token) {
                // Empty.
            }

            const Order         ORDER         = Order::BINARY;
            const Precedence    PRECEDENCE    = Precedence::POWER;
            const Associativity ASSOCIATIVITY = Associativity::RIGHT;

            std::string debug_repr() override { return "op ^"; }
        };

        /**
         * @brief Node reflecting a Multiplication operator.
         *
         * @inedge operator | paren expression | assignment | block expression
         * @outedge rvalue expression
         * @outedge rvalue expression
         */
        struct MultiplicationOperatorNode : public OperatorNode {
            MultiplicationOperatorNode(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) :
                OperatorNode(_first_token, _last_token) {
                // Empty.
            }

            const Order         ORDER         = Order::BINARY;
            const Precedence    PRECEDENCE    = Precedence::FACTOR;
            const Associativity ASSOCIATIVITY = Associativity::LEFT;

            std::string debug_repr() override { return "op *"; }
        };

        /**
         * @brief Node reflecting a Division operator.
         *
         * @inedge operator | paren expression | assignment | block expression
         * @outedge rvalue expression
         * @outedge rvalue expression
         */
        struct DivisionOperatorNode : public OperatorNode {
            DivisionOperatorNode(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) :
                OperatorNode(_first_token, _last_token) {
                // Empty.
            }

            const Order         ORDER         = Order::BINARY;
            const Precedence    PRECEDENCE    = Precedence::FACTOR;
            const Associativity ASSOCIATIVITY = Associativity::LEFT;

            std::string debug_repr() override { return "op /"; }
        };

        /**
         * @brief Node reflecting an Addition operator.
         *
         * @inedge operator | paren expression | assignment | block expression
         * @outedge rvalue expression
         * @outedge rvalue expression
         */
        struct AdditionOperatorNode : public OperatorNode {
            AdditionOperatorNode(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) :
                OperatorNode(_first_token, _last_token) {
                // Empty.
            }

            const Order         ORDER         = Order::BINARY;
            const Precedence    PRECEDENCE    = Precedence::TERM;
            const Associativity ASSOCIATIVITY = Associativity::LEFT;

            std::string debug_repr() override { return "op +"; }
        };

        /**
         * @brief Node reflecting a Subtraction operator.
         *
         * @inedge operator | paren expression | assignment | block expression
         * @outedge rvalue expression
         * @outedge rvalue expression
         */
        struct SubtractionOperatorNode : public OperatorNode {
            SubtractionOperatorNode(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) :
                OperatorNode(_first_token, _last_token) {
                // Empty.
            }

            const Order         ORDER         = Order::BINARY;
            const Precedence    PRECEDENCE    = Precedence::TERM;
            const Associativity ASSOCIATIVITY = Associativity::LEFT;

            std::string debug_repr() override { return "op -"; }
        };

        /**
         * @brief Node reflecting a Greater operator.
         *
         * @inedge operator | paren expression | assignment | block expression
         * @outedge rvalue expression
         * @outedge rvalue expression
         */
        struct GreaterOperatorNode : public OperatorNode {
            GreaterOperatorNode(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) :
                OperatorNode(_first_token, _last_token) {
                // Empty.
            }

            const Order         ORDER         = Order::BINARY;
            const Precedence    PRECEDENCE    = Precedence::COMPARISON;
            const Associativity ASSOCIATIVITY = Associativity::NONE;

            std::string debug_repr() override { return "op >"; }
        };

        /**
         * @brief Node reflecting a GreaterOrEqual operator.
         *
         * @inedge operator | paren expression | assignment | block expression
         * @outedge rvalue expression
         * @outedge rvalue expression
         */
        struct GreaterOrEqualOperatorNode : public OperatorNode {
            GreaterOrEqualOperatorNode(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) :
                OperatorNode(_first_token, _last_token) {
                // Empty.
            }

            const Order         ORDER         = Order::BINARY;
            const Precedence    PRECEDENCE    = Precedence::COMPARISON;
            const Associativity ASSOCIATIVITY = Associativity::NONE;

            std::string debug_repr() override { return "op >="; }
        };

        /**
         * @brief Node reflecting a Lesser operator.
         *
         * @inedge operator | paren expression | assignment | block expression
         * @outedge rvalue expression
         * @outedge rvalue expression
         */
        struct LesserOperatorNode : public OperatorNode {
            LesserOperatorNode(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) :
                OperatorNode(_first_token, _last_token) {
                // Empty.
            }

            const Order         ORDER         = Order::BINARY;
            const Precedence    PRECEDENCE    = Precedence::COMPARISON;
            const Associativity ASSOCIATIVITY = Associativity::NONE;

            std::string debug_repr() override { return "op <"; }
        };

        /**
         * @brief Node reflecting a LesserOrEqual operator.
         *
         * @inedge operator | paren expression | assignment | block expression
         * @outedge rvalue expression
         * @outedge rvalue expression
         */
        struct LesserOrEqualOperatorNode : public OperatorNode {
            LesserOrEqualOperatorNode(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) :
                OperatorNode(_first_token, _last_token) {
                // Empty.
            }

            const Order         ORDER         = Order::BINARY;
            const Precedence    PRECEDENCE    = Precedence::COMPARISON;
            const Associativity ASSOCIATIVITY = Associativity::NONE;

            std::string debug_repr() override { return "op <="; }
        };

        /**
         * @brief Node reflecting an Equal operator.
         *
         * @inedge operator | paren expression | assignment | block expression
         * @outedge rvalue expression
         * @outedge rvalue expression
         */
        struct EqualOperatorNode : public OperatorNode {
            EqualOperatorNode(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) :
                OperatorNode(_first_token, _last_token) {
                // Empty.
            }

            const Order         ORDER         = Order::BINARY;
            const Precedence    PRECEDENCE    = Precedence::EQUALITY;
            const Associativity ASSOCIATIVITY = Associativity::LEFT;

            std::string debug_repr() override { return "op =="; }
        };

        /**
         * @brief Node reflecting a NotEqual operator.
         *
         * @inedge operator | paren expression | assignment | block expression
         * @outedge rvalue expression
         * @outedge rvalue expression
         */
        struct NotEqualOperatorNode : public OperatorNode {
            NotEqualOperatorNode(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) :
                OperatorNode(_first_token, _last_token) {
                // Empty.
            }

            const Order         ORDER         = Order::BINARY;
            const Precedence    PRECEDENCE    = Precedence::EQUALITY;
            const Associativity ASSOCIATIVITY = Associativity::LEFT;

            std::string debug_repr() override { return "op !="; }
        };

        /**
         * @brief Node reflecting an Or operator.
         *
         * @inedge operator | paren expression | assignment | block expression
         * @outedge rvalue expression
         * @outedge rvalue expression
         */
        struct OrOperatorNode : public OperatorNode {
            OrOperatorNode(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) :
                OperatorNode(_first_token, _last_token) {
                // Empty.
            }

            const Order         ORDER         = Order::BINARY;
            const Precedence    PRECEDENCE    = Precedence::LOGICAL;
            const Associativity ASSOCIATIVITY = Associativity::LEFT;

            std::string debug_repr() override { return "op or"; }
        };

        /**
         * @brief Node reflecting an And operator.
         *
         * @inedge operator | paren expression | assignment | block expression
         * @outedge rvalue expression
         * @outedge rvalue expression
         */
        struct AndOperatorNode : public OperatorNode {
            AndOperatorNode(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) :
                OperatorNode(_first_token, _last_token) {
                // Empty.
            }

            const Order         ORDER         = Order::BINARY;
            const Precedence    PRECEDENCE    = Precedence::LOGICAL;
            const Associativity ASSOCIATIVITY = Associativity::LEFT;

            std::string debug_repr() override { return "op and"; }
        };

        /**
         * @brief Node reflecting a value assignment operator.
         *
         * @inedge lvalue expression
         * @outedge rvalue expression
         */
        struct AssignmentOperatorNode : public OperatorNode {
            AssignmentOperatorNode(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) :
                OperatorNode(_first_token, _last_token) {
                // Empty.
            }

            const Order         ORDER         = Order::BINARY;
            const Precedence    PRECEDENCE    = Precedence::ASSIGNMENT;
            const Associativity ASSOCIATIVITY = Associativity::RIGHT;

            std::string debug_repr() override { return "op ="; }
        };
    }  // namespace ast
}  // namespace mattflow
namespace mfast = mattflow::ast;

#endif  // __mattflow_ast_node_operator_h
