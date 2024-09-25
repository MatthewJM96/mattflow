#ifndef __mattflow_ast_node_type_h
#define __mattflow_ast_node_type_h

#include "ast/node/node.h"

namespace mattflow {
    namespace ast {
        /**
         * @brief Node reflecting the Boolean type.
         *
         * @inedge TODO
         */
        struct BoolNode : public Node {
            BoolNode(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) :
                Node(_first_token, _last_token) {
                // Empty.
            }

            std::string debug_repr() const override { return "type: bool"; }
        };

        /**
         * @brief Node reflecting the char type.
         *
         * @inedge TODO
         */
        struct CharNode : public Node {
            CharNode(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) :
                Node(_first_token, _last_token) {
                // Empty.
            }

            std::string debug_repr() const override { return "type: char"; }
        };

        /**
         * @brief Node reflecting the int type.
         *
         * @inedge TODO
         */
        struct IntNode : public Node {
            IntNode(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) :
                Node(_first_token, _last_token) {
                // Empty.
            }

            std::string debug_repr() const override { return "type: int"; }
        };

        /**
         * @brief Node reflecting the int 8-bit type.
         *
         * @inedge TODO
         */
        struct Int8Node : public Node {
            Int8Node(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) :
                Node(_first_token, _last_token) {
                // Empty.
            }

            std::string debug_repr() const override { return "type: int8"; }
        };

        /**
         * @brief Node reflecting the int 16-bit type.
         *
         * @inedge TODO
         */
        struct Int16Node : public Node {
            Int16Node(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) :
                Node(_first_token, _last_token) {
                // Empty.
            }

            std::string debug_repr() const override { return "type: int16"; }
        };

        /**
         * @brief Node reflecting the int 32-bit type.
         *
         * @inedge TODO
         */
        struct Int32Node : public Node {
            Int32Node(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) :
                Node(_first_token, _last_token) {
                // Empty.
            }

            std::string debug_repr() const override { return "type: int32"; }
        };

        /**
         * @brief Node reflecting the int 64-bit type.
         *
         * @inedge TODO
         */
        struct Int64Node : public Node {
            Int64Node(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) :
                Node(_first_token, _last_token) {
                // Empty.
            }

            std::string debug_repr() const override { return "type: int64"; }
        };

        /**
         * @brief Node reflecting the unsigned int type.
         *
         * @inedge TODO
         */
        struct UIntNode : public Node {
            UIntNode(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) :
                Node(_first_token, _last_token) {
                // Empty.
            }

            std::string debug_repr() const override { return "type: uint"; }
        };

        /**
         * @brief Node reflecting the unsigned int 8-bit type.
         *
         * @inedge TODO
         */
        struct UInt8Node : public Node {
            UInt8Node(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) :
                Node(_first_token, _last_token) {
                // Empty.
            }

            std::string debug_repr() const override { return "type: uint8"; }
        };

        /**
         * @brief Node reflecting the unsigned int 16-bit type.
         *
         * @inedge TODO
         */
        struct UInt16Node : public Node {
            UInt16Node(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) :
                Node(_first_token, _last_token) {
                // Empty.
            }

            std::string debug_repr() const override { return "type: uint16"; }
        };

        /**
         * @brief Node reflecting the unsigned int 32-bit type.
         *
         * @inedge TODO
         */
        struct UInt32Node : public Node {
            UInt32Node(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) :
                Node(_first_token, _last_token) {
                // Empty.
            }

            std::string debug_repr() const override { return "type: uint32"; }
        };

        /**
         * @brief Node reflecting the unsigned int 64-bit type.
         *
         * @inedge TODO
         */
        struct UInt64Node : public Node {
            UInt64Node(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) :
                Node(_first_token, _last_token) {
                // Empty.
            }

            std::string debug_repr() const override { return "type: uint64"; }
        };

        /**
         * @brief Node reflecting the float 64-bit type.
         *
         * @inedge TODO
         */
        struct Float32Node : public Node {
            Float32Node(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) :
                Node(_first_token, _last_token) {
                // Empty.
            }

            std::string debug_repr() const override { return "type: float32"; }
        };

        /**
         * @brief Node reflecting the float 64-bit type.
         *
         * @inedge TODO
         */
        struct Float64Node : public Node {
            Float64Node(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) :
                Node(_first_token, _last_token) {
                // Empty.
            }

            std::string debug_repr() const override { return "type: float64"; }
        };
    }  // namespace ast
}  // namespace mattflow
namespace mfast = mattflow::ast;

#endif  // __mattflow_ast_node_type_h
