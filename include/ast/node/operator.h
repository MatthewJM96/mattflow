#ifndef __mattflow_ast_node_operator_h
#define __mattflow_ast_node_operator_h

#include "ast/node/node.h"
#include "lex/token_type.h"

namespace mattflow {
    namespace ast {
        enum class Order {
            NONE,
            UNARY,
            BINARY
        };

        enum class Precedence : int16_t {
            NONE             = 0,
            VALUE_ASSIGNMENT = 100,
            ARROW            = 200,
            SEQUENCE         = 300,
            COMMA            = 300,
            RANGE_CONSTRAINT = 400,
            RANGE            = 500,
            LOGICAL          = 600,
            EQUALITY         = 700,
            COMPARISON       = 800,
            TERM             = 900,
            FACTOR           = 1000,
            POWER            = 1100,
            UNARY            = 1200,
            TYPE_ASSIGNMENT  = 1300,
            DOT              = 1400
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

            virtual ~OperatorNode() {
                // Empty.
            }
        };

        /**
         * @brief Node reflecting a Dot operator.
         *
         * @inedge operator | paren expression | assignment | block expression
         * @outedge rvalue expression
         */
        struct DotOperatorNode : public OperatorNode {
            DotOperatorNode(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) :
                OperatorNode(_first_token, _last_token) {
                // Empty.
            }

            virtual ~DotOperatorNode() {
                // Empty.
            }

            const Order         ORDER         = Order::BINARY;
            const Precedence    PRECEDENCE    = Precedence::DOT;
            const Associativity ASSOCIATIVITY = Associativity::LEFT;

            std::string debug_repr() const override { return "op dot"; }
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

            virtual ~NotOperatorNode() {
                // Empty.
            }

            const Order         ORDER         = Order::UNARY;
            const Precedence    PRECEDENCE    = Precedence::UNARY;
            const Associativity ASSOCIATIVITY = Associativity::RIGHT;

            std::string debug_repr() const override { return "op !"; }
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

            virtual ~NegationOperatorNode() {
                // Empty.
            }

            const Order         ORDER         = Order::UNARY;
            const Precedence    PRECEDENCE    = Precedence::UNARY;
            const Associativity ASSOCIATIVITY = Associativity::RIGHT;

            std::string debug_repr() const override { return "op -"; }
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

            virtual ~PowerOperatorNode() {
                // Empty.
            }

            const Order         ORDER         = Order::BINARY;
            const Precedence    PRECEDENCE    = Precedence::POWER;
            const Associativity ASSOCIATIVITY = Associativity::RIGHT;

            std::string debug_repr() const override { return "op ^"; }
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

            virtual ~MultiplicationOperatorNode() {
                // Empty.
            }

            const Order         ORDER         = Order::BINARY;
            const Precedence    PRECEDENCE    = Precedence::FACTOR;
            const Associativity ASSOCIATIVITY = Associativity::LEFT;

            std::string debug_repr() const override { return "op *"; }
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

            virtual ~DivisionOperatorNode() {
                // Empty.
            }

            const Order         ORDER         = Order::BINARY;
            const Precedence    PRECEDENCE    = Precedence::FACTOR;
            const Associativity ASSOCIATIVITY = Associativity::LEFT;

            std::string debug_repr() const override { return "op /"; }
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

            virtual ~AdditionOperatorNode() {
                // Empty.
            }

            const Order         ORDER         = Order::BINARY;
            const Precedence    PRECEDENCE    = Precedence::TERM;
            const Associativity ASSOCIATIVITY = Associativity::LEFT;

            std::string debug_repr() const override { return "op +"; }
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

            virtual ~SubtractionOperatorNode() {
                // Empty.
            }

            const Order         ORDER         = Order::BINARY;
            const Precedence    PRECEDENCE    = Precedence::TERM;
            const Associativity ASSOCIATIVITY = Associativity::LEFT;

            std::string debug_repr() const override { return "op -"; }
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

            virtual ~GreaterOperatorNode() {
                // Empty.
            }

            const Order         ORDER         = Order::BINARY;
            const Precedence    PRECEDENCE    = Precedence::COMPARISON;
            const Associativity ASSOCIATIVITY = Associativity::NONE;

            std::string debug_repr() const override { return "op >"; }
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

            virtual ~GreaterOrEqualOperatorNode() {
                // Empty.
            }

            const Order         ORDER         = Order::BINARY;
            const Precedence    PRECEDENCE    = Precedence::COMPARISON;
            const Associativity ASSOCIATIVITY = Associativity::NONE;

            std::string debug_repr() const override { return "op >="; }
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

            virtual ~LesserOperatorNode() {
                // Empty.
            }

            const Order         ORDER         = Order::BINARY;
            const Precedence    PRECEDENCE    = Precedence::COMPARISON;
            const Associativity ASSOCIATIVITY = Associativity::NONE;

            std::string debug_repr() const override { return "op <"; }
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

            virtual ~LesserOrEqualOperatorNode() {
                // Empty.
            }

            const Order         ORDER         = Order::BINARY;
            const Precedence    PRECEDENCE    = Precedence::COMPARISON;
            const Associativity ASSOCIATIVITY = Associativity::NONE;

            std::string debug_repr() const override { return "op <="; }
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

            virtual ~EqualOperatorNode() {
                // Empty.
            }

            const Order         ORDER         = Order::BINARY;
            const Precedence    PRECEDENCE    = Precedence::EQUALITY;
            const Associativity ASSOCIATIVITY = Associativity::LEFT;

            std::string debug_repr() const override { return "op =="; }
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

            virtual ~NotEqualOperatorNode() {
                // Empty.
            }

            const Order         ORDER         = Order::BINARY;
            const Precedence    PRECEDENCE    = Precedence::EQUALITY;
            const Associativity ASSOCIATIVITY = Associativity::LEFT;

            std::string debug_repr() const override { return "op !="; }
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

            virtual ~OrOperatorNode() {
                // Empty.
            }

            const Order         ORDER         = Order::BINARY;
            const Precedence    PRECEDENCE    = Precedence::LOGICAL;
            const Associativity ASSOCIATIVITY = Associativity::LEFT;

            std::string debug_repr() const override { return "op or"; }
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

            virtual ~AndOperatorNode() {
                // Empty.
            }

            const Order         ORDER         = Order::BINARY;
            const Precedence    PRECEDENCE    = Precedence::LOGICAL;
            const Associativity ASSOCIATIVITY = Associativity::LEFT;

            std::string debug_repr() const override { return "op and"; }
        };

        /**
         * @brief Node reflecting an Range operator.
         *
         * @inedge operator | paren expression | assignment | block expression
         * @outedge rvalue expression
         * @outedge rvalue expression
         */
        struct RangeOperatorNode : public OperatorNode {
            RangeOperatorNode(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) :
                OperatorNode(_first_token, _last_token) {
                // Empty.
            }

            virtual ~RangeOperatorNode() {
                // Empty.
            }

            const Order         ORDER         = Order::BINARY;
            const Precedence    PRECEDENCE    = Precedence::RANGE;
            const Associativity ASSOCIATIVITY = Associativity::LEFT;

            std::string debug_repr() const override { return "op range"; }
        };

        /**
         * @brief Node reflecting an RangeConstraint operator.
         *
         * @inedge operator | paren expression | assignment | block expression
         * @outedge rvalue expression
         * @outedge rvalue expression
         */
        struct RangeConstraintOperatorNode : public OperatorNode {
            RangeConstraintOperatorNode(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) :
                OperatorNode(_first_token, _last_token) {
                // Empty.
            }

            virtual ~RangeConstraintOperatorNode() {
                // Empty.
            }

            const Order         ORDER         = Order::BINARY;
            const Precedence    PRECEDENCE    = Precedence::RANGE;
            const Associativity ASSOCIATIVITY = Associativity::LEFT;

            std::string debug_repr() const override { return "op range constraint"; }
        };

        /**
         * @brief Node reflecting an Sequence operator.
         *
         * @inedge operator | paren expression | assignment | block expression
         * @outedge rvalue expression
         * @outedge rvalue expression
         */
        struct SequenceOperatorNode : public OperatorNode {
            SequenceOperatorNode(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) :
                OperatorNode(_first_token, _last_token) {
                // Empty.
            }

            virtual ~SequenceOperatorNode() {
                // Empty.
            }

            const Order         ORDER         = Order::BINARY;
            const Precedence    PRECEDENCE    = Precedence::SEQUENCE;
            const Associativity ASSOCIATIVITY = Associativity::LEFT;

            std::string debug_repr() const override { return "op sequence"; }
        };

        /**
         * @brief Node reflecting an Comma operator.
         *
         * @inedge operator | paren expression | assignment | block expression
         * @outedge rvalue expression
         * @outedge rvalue expression
         */
        struct CommaOperatorNode : public OperatorNode {
            CommaOperatorNode(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) :
                OperatorNode(_first_token, _last_token) {
                // Empty.
            }

            virtual ~CommaOperatorNode() {
                // Empty.
            }

            const Order         ORDER         = Order::BINARY;
            const Precedence    PRECEDENCE    = Precedence::COMMA;
            const Associativity ASSOCIATIVITY = Associativity::LEFT;

            std::string debug_repr() const override { return "op comma"; }
        };

        /**
         * @brief Node reflecting a deduced value assignment operator.
         *
         * @inedge lvalue expression
         * @outedge rvalue expression
         */
        struct AssignDeducedValueOperatorNode : public OperatorNode {
            AssignDeducedValueOperatorNode(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) :
                OperatorNode(_first_token, _last_token) {
                // Empty.
            }

            virtual ~AssignDeducedValueOperatorNode() {
                // Empty.
            }

            const Order         ORDER         = Order::BINARY;
            const Precedence    PRECEDENCE    = Precedence::VALUE_ASSIGNMENT;
            const Associativity ASSOCIATIVITY = Associativity::NONE;

            std::string debug_repr() const override { return "op :="; }
        };

        /**
         * @brief Node reflecting a value assignment operator.
         *
         * @inedge lvalue expression
         * @outedge rvalue expression
         */
        struct AssignValueOperatorNode : public OperatorNode {
            AssignValueOperatorNode(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) :
                OperatorNode(_first_token, _last_token) {
                // Empty.
            }

            virtual ~AssignValueOperatorNode() {
                // Empty.
            }

            const Order         ORDER         = Order::BINARY;
            const Precedence    PRECEDENCE    = Precedence::VALUE_ASSIGNMENT;
            const Associativity ASSOCIATIVITY = Associativity::RIGHT;

            std::string debug_repr() const override { return "op ="; }
        };

        /**
         * @brief Node reflecting a type assignment operator.
         *
         * @inedge lvalue expression
         * @outedge rvalue expression
         */
        struct AssignTypeOperatorNode : public OperatorNode {
            AssignTypeOperatorNode(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) :
                OperatorNode(_first_token, _last_token) {
                // Empty.
            }

            virtual ~AssignTypeOperatorNode() {
                // Empty.
            }

            const Order         ORDER         = Order::BINARY;
            const Precedence    PRECEDENCE    = Precedence::TYPE_ASSIGNMENT;
            const Associativity ASSOCIATIVITY = Associativity::NONE;

            std::string debug_repr() const override { return "op :"; }
        };

        /**
         * @brief Node reflecting a deduced-type arrow operator.
         *
         * @inedge lvalue expression
         * @outedge rvalue expression
         */
        struct DeducedArrowOperatorNode : public OperatorNode {
            DeducedArrowOperatorNode(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) :
                OperatorNode(_first_token, _last_token) {
                // Empty.
            }

            virtual ~DeducedArrowOperatorNode() {
                // Empty.
            }

            const Order         ORDER         = Order::BINARY;
            const Precedence    PRECEDENCE    = Precedence::ARROW;
            const Associativity ASSOCIATIVITY = Associativity::NONE;

            std::string debug_repr() const override { return "op :->"; }
        };

        /**
         * @brief Node reflecting an arrow operator.
         *
         * @inedge lvalue expression
         * @outedge rvalue expression
         */
        struct ArrowOperatorNode : public OperatorNode {
            ArrowOperatorNode(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) :
                OperatorNode(_first_token, _last_token) {
                // Empty.
            }

            virtual ~ArrowOperatorNode() {
                // Empty.
            }

            const Order         ORDER         = Order::BINARY;
            const Precedence    PRECEDENCE    = Precedence::ARROW;
            const Associativity ASSOCIATIVITY = Associativity::NONE;

            std::string debug_repr() const override { return "op ->"; }
        };
    }  // namespace ast
}  // namespace mattflow
namespace mfast = mattflow::ast;

#endif  // __mattflow_ast_node_operator_h
