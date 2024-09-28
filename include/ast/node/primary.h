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
            NullNode(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) :
                Node(_first_token, _last_token) {
                // Empty.
            }

            virtual ~NullNode() {
                // Empty.
            }

            std::string debug_repr() const override { return "null"; }
        };

        /**
         * @brief Node reflecting a boolean literal.
         *
         * @inedge operator | paren expression | assignment | block expression
         */
        struct BoolValNode : public Node {
            BoolValNode(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token,
                bool                          _value
            ) :
                Node(_first_token, _last_token), value(_value) {
                // Empty.
            }

            virtual ~BoolValNode() {
                // Empty.
            }

            bool value;

            std::string debug_repr() const override {
                return "bool: " + std::string(value ? "true" : "false");
            }
        };

        /**
         * @brief Node reflecting a number literal.
         *
         * @inedge operator | paren expression | assignment | block expression
         */
        struct NumberValNode : public Node {
            NumberValNode(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token,
                mflit::Number                 _value
            ) :
                Node(_first_token, _last_token), value(_value) {
                // Empty.
            }

            virtual ~NumberValNode() {
                // Empty.
            }

            mflit::Number value;

            std::string debug_repr() const override {
                return "num: "
                       + std::to_string(
                           value.is_floating_point() ? value.template as<float>() :
                                                       value.template as<int>()
                       );
            }
        };

        /**
         * @brief Node reflecting a string literal.
         *
         * @inedge operator | paren expression | assignment | block expression
         */
        struct StringValNode : public Node {
            StringValNode(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token,
                mflit::StringIdx              _value
            ) :
                Node(_first_token, _last_token), value(_value) {
                // Empty.
            }

            virtual ~StringValNode() {
                // Empty.
            }

            mflit::StringIdx value;

            std::string debug_repr() const override {
                static auto& str_table = mflit::StringTable::get();
                return "str: " + std::string(str_table.get(value));
            }
        };

        /**
         * @brief Node reflecting an identifier literal.
         *
         * @inedge block expression
         * @outedge type expression
         * @outedge rvalue expression
         */
        struct IdentifierNode : public Node {
            IdentifierNode(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token,
                mflit::IdentifierIdx          _name
            ) :
                Node(_first_token, _last_token), name(_name) {
                // Empty.
            }

            virtual ~IdentifierNode() {
                // Empty.
            }

            mflit::IdentifierIdx name;

            std::string debug_repr() const override {
                static auto& ident_table = mflit::IdentifierTable::get();
                return "ident: " + std::string(ident_table.get(name));
            }
        };
    }  // namespace ast
}  // namespace mattflow
namespace mfast = mattflow::ast;

#endif  // __mattflow_ast_node_number_h
