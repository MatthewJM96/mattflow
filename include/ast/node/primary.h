#ifndef __mattflow_ast_node_number_h
#define __mattflow_ast_node_number_h

#include "ast/node/node.h"
#include "literal/number.h"

namespace mattflow {
    namespace ast {
        /**
         * @brief Node reflecting a null literal.
         *
         * @inedge operator | paren expression | assignment | block expression
         */
        struct NullNode : public Node {
            // Empty.
            NullNode(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) :
                Node(_first_token, _last_token) { }

            NullNode(const NullNode& node) : Node(node) { }

            NullNode(NullNode&& node) : Node(node) { }
        };

        /**
         * @brief Node reflecting a boolean literal.
         *
         * @inedge operator | paren expression | assignment | block expression
         */
        struct BoolNode : public Node {
            bool value;

            BoolNode(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token,
                bool                          _value
            ) :
                Node(_first_token, _last_token) {
                value = _value;
            }

            BoolNode(const BoolNode& node) : Node(node) { value = node.value; }

            BoolNode(BoolNode&& node) : Node(node) { value = node.value; }
        };

        /**
         * @brief Node reflecting a number literal.
         *
         * @inedge operator | paren expression | assignment | block expression
         */
        struct NumberNode : public Node {
            mflit::Number value;

            NumberNode(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token,
                mflit::Number                 _value
            ) :
                Node(_first_token, _last_token) {
                value = _value;
            }

            NumberNode(const NumberNode& node) : Node(node) { value = node.value; }

            NumberNode(NumberNode&& node) : Node(node) { value = node.value; }
        };

        /**
         * @brief Node reflecting a string literal.
         *
         * @inedge operator | paren expression | assignment | block expression
         */
        struct StringNode : public Node {
            mflit::StringIdx value;

            StringNode(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token,
                mflit::StringIdx              _value
            ) :
                Node(_first_token, _last_token) {
                value = _value;
            }

            StringNode(const StringNode& node) : Node(node) { value = node.value; }

            StringNode(StringNode&& node) : Node(node) { value = node.value; }
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

            IdentifierNode(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token,
                mflit::IdentifierIdx          _name
            ) :
                Node(_first_token, _last_token) {
                name = _name;
            }

            IdentifierNode(const IdentifierNode& node) : Node(node) {
                name = node.name;
            }

            IdentifierNode(IdentifierNode&& node) : Node(node) { name = node.name; }
        };
    }  // namespace ast
}  // namespace mattflow
namespace mfast = mattflow::ast;

#endif  // __mattflow_ast_node_number_h
