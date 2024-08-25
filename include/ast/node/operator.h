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
            NotOperatorNode(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) :
                Node(_first_token, _last_token) { }

            NotOperatorNode(const NotOperatorNode& node) : Node(node) { }

            NotOperatorNode(NotOperatorNode&& node) : Node(node) { }
        };

        /**
         * @brief Node reflecting a Negation operator.
         *
         * @inedge operator | paren expression | assignment | block expression
         * @outedge rvalue expression
         */
        struct NegationOperatorNode : public Node {
            NegationOperatorNode(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) :
                Node(_first_token, _last_token) { }

            NegationOperatorNode(const NegationOperatorNode& node) : Node(node) { }

            NegationOperatorNode(NegationOperatorNode&& node) : Node(node) { }
        };

        /**
         * @brief Node reflecting a Multiplication operator.
         *
         * @inedge operator | paren expression | assignment | block expression
         * @outedge rvalue expression
         * @outedge rvalue expression
         */
        struct MultiplicationOperatorNode : public Node {
            MultiplicationOperatorNode(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) :
                Node(_first_token, _last_token) { }

            MultiplicationOperatorNode(const MultiplicationOperatorNode& node) :
                Node(node) { }

            MultiplicationOperatorNode(MultiplicationOperatorNode&& node) :
                Node(node) { }
        };

        /**
         * @brief Node reflecting a Division operator.
         *
         * @inedge operator | paren expression | assignment | block expression
         * @outedge rvalue expression
         * @outedge rvalue expression
         */
        struct DivisionOperatorNode : public Node {
            DivisionOperatorNode(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) :
                Node(_first_token, _last_token) { }

            DivisionOperatorNode(const DivisionOperatorNode& node) : Node(node) { }

            DivisionOperatorNode(DivisionOperatorNode&& node) : Node(node) { }
        };

        /**
         * @brief Node reflecting an Addition operator.
         *
         * @inedge operator | paren expression | assignment | block expression
         * @outedge rvalue expression
         * @outedge rvalue expression
         */
        struct AdditionOperatorNode : public Node {
            AdditionOperatorNode(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) :
                Node(_first_token, _last_token) { }

            AdditionOperatorNode(const AdditionOperatorNode& node) : Node(node) { }

            AdditionOperatorNode(AdditionOperatorNode&& node) : Node(node) { }
        };

        /**
         * @brief Node reflecting a Subtraction operator.
         *
         * @inedge operator | paren expression | assignment | block expression
         * @outedge rvalue expression
         * @outedge rvalue expression
         */
        struct SubtractionOperatorNode : public Node {
            SubtractionOperatorNode(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) :
                Node(_first_token, _last_token) { }

            SubtractionOperatorNode(const SubtractionOperatorNode& node) :
                Node(node) { }

            SubtractionOperatorNode(SubtractionOperatorNode&& node) : Node(node) { }
        };

        /**
         * @brief Node reflecting a Greater operator.
         *
         * @inedge operator | paren expression | assignment | block expression
         * @outedge rvalue expression
         * @outedge rvalue expression
         */
        struct GreaterOperatorNode : public Node {
            GreaterOperatorNode(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) :
                Node(_first_token, _last_token) { }

            GreaterOperatorNode(const GreaterOperatorNode& node) : Node(node) { }

            GreaterOperatorNode(GreaterOperatorNode&& node) : Node(node) { }
        };

        /**
         * @brief Node reflecting a GreaterOrEqual operator.
         *
         * @inedge operator | paren expression | assignment | block expression
         * @outedge rvalue expression
         * @outedge rvalue expression
         */
        struct GreaterOrEqualOperatorNode : public Node {
            GreaterOrEqualOperatorNode(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) :
                Node(_first_token, _last_token) { }

            GreaterOrEqualOperatorNode(const GreaterOrEqualOperatorNode& node) :
                Node(node) { }

            GreaterOrEqualOperatorNode(GreaterOrEqualOperatorNode&& node) :
                Node(node) { }
        };

        /**
         * @brief Node reflecting a Lesser operator.
         *
         * @inedge operator | paren expression | assignment | block expression
         * @outedge rvalue expression
         * @outedge rvalue expression
         */
        struct LesserOperatorNode : public Node {
            LesserOperatorNode(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) :
                Node(_first_token, _last_token) { }

            LesserOperatorNode(const LesserOperatorNode& node) : Node(node) { }

            LesserOperatorNode(LesserOperatorNode&& node) : Node(node) { }
        };

        /**
         * @brief Node reflecting a LesserOrEqual operator.
         *
         * @inedge operator | paren expression | assignment | block expression
         * @outedge rvalue expression
         * @outedge rvalue expression
         */
        struct LesserOrEqualOperatorNode : public Node {
            LesserOrEqualOperatorNode(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) :
                Node(_first_token, _last_token) { }

            LesserOrEqualOperatorNode(const LesserOrEqualOperatorNode& node) :
                Node(node) { }

            LesserOrEqualOperatorNode(LesserOrEqualOperatorNode&& node) : Node(node) { }
        };

        /**
         * @brief Node reflecting an Equal operator.
         *
         * @inedge operator | paren expression | assignment | block expression
         * @outedge rvalue expression
         * @outedge rvalue expression
         */
        struct EqualOperatorNode : public Node {
            EqualOperatorNode(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) :
                Node(_first_token, _last_token) { }

            EqualOperatorNode(const EqualOperatorNode& node) : Node(node) { }

            EqualOperatorNode(EqualOperatorNode&& node) : Node(node) { }
        };

        /**
         * @brief Node reflecting a NotEqual operator.
         *
         * @inedge operator | paren expression | assignment | block expression
         * @outedge rvalue expression
         * @outedge rvalue expression
         */
        struct NotEqualOperatorNode : public Node {
            NotEqualOperatorNode(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) :
                Node(_first_token, _last_token) { }

            NotEqualOperatorNode(const NotEqualOperatorNode& node) : Node(node) { }

            NotEqualOperatorNode(NotEqualOperatorNode&& node) : Node(node) { }
        };

        /**
         * @brief Node reflecting an Or operator.
         *
         * @inedge operator | paren expression | assignment | block expression
         * @outedge rvalue expression
         * @outedge rvalue expression
         */
        struct OrOperatorNode : public Node {
            OrOperatorNode(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) :
                Node(_first_token, _last_token) { }

            OrOperatorNode(const OrOperatorNode& node) : Node(node) { }

            OrOperatorNode(OrOperatorNode&& node) : Node(node) { }
        };

        /**
         * @brief Node reflecting an And operator.
         *
         * @inedge operator | paren expression | assignment | block expression
         * @outedge rvalue expression
         * @outedge rvalue expression
         */
        struct AndOperatorNode : public Node {
            AndOperatorNode(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) :
                Node(_first_token, _last_token) { }

            AndOperatorNode(const AndOperatorNode& node) : Node(node) { }

            AndOperatorNode(AndOperatorNode&& node) : Node(node) { }
        };

        /**
         * @brief Node reflecting a value assignment operator.
         *
         * @inedge lvalue expression
         * @outedge rvalue expression
         */
        struct AssignmentOperatorNode : public Node {
            AssignmentOperatorNode(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) :
                Node(_first_token, _last_token) { }

            AssignmentOperatorNode(const AssignmentOperatorNode& node) : Node(node) { }

            AssignmentOperatorNode(AssignmentOperatorNode&& node) : Node(node) { }
        };
    }  // namespace ast
}  // namespace mattflow
namespace mfast = mattflow::ast;

#endif  // __mattflow_ast_node_operator_h
