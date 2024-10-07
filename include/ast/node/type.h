#ifndef __mattflow_ast_node_type_h
#define __mattflow_ast_node_type_h

#include "ast/node/node.h"

#include "../../type.h"

namespace mattflow {
    namespace ast {
        /**
         * @brief Node reflecting a type.
         */
        struct TypeNode : public Node {
            TypeNode(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) :
                Node(_first_token, _last_token) {
                // Empty.
            }

            virtual ~TypeNode() {
                // Empty.
            }
        };

        /**
         * @brief Node reflecting the Boolean type.
         *
         * @inedge TODO
         */
        struct BoolNode : public TypeNode {
            BoolNode(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) :
                TypeNode(_first_token, _last_token) {
                // Empty.
            }

            virtual ~BoolNode() {
                // Empty.
            }

            constexpr static mftype::Type TYPE = mftype::PrimitiveType::BOOL;

            std::string debug_repr() const override { return "type: bool"; }
        };

        /**
         * @brief Node reflecting the char type.
         *
         * @inedge TODO
         */
        struct CharNode : public TypeNode {
            CharNode(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) :
                TypeNode(_first_token, _last_token) {
                // Empty.
            }

            virtual ~CharNode() {
                // Empty.
            }

            constexpr static mftype::Type TYPE = mftype::PrimitiveType::CHAR;

            std::string debug_repr() const override { return "type: char"; }
        };

        /**
         * @brief Node reflecting the int type.
         *
         * @inedge TODO
         */
        struct IntNode : public TypeNode {
            IntNode(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) :
                TypeNode(_first_token, _last_token) {
                // Empty.
            }

            virtual ~IntNode() {
                // Empty.
            }

            constexpr static mftype::Type TYPE = mftype::PrimitiveType::INT;

            std::string debug_repr() const override { return "type: int"; }
        };

        /**
         * @brief Node reflecting the int 8-bit type.
         *
         * @inedge TODO
         */
        struct Int8Node : public TypeNode {
            Int8Node(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) :
                TypeNode(_first_token, _last_token) {
                // Empty.
            }

            virtual ~Int8Node() {
                // Empty.
            }

            constexpr static mftype::Type TYPE = mftype::PrimitiveType::INT8;

            std::string debug_repr() const override { return "type: int8"; }
        };

        /**
         * @brief Node reflecting the int 16-bit type.
         *
         * @inedge TODO
         */
        struct Int16Node : public TypeNode {
            Int16Node(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) :
                TypeNode(_first_token, _last_token) {
                // Empty.
            }

            virtual ~Int16Node() {
                // Empty.
            }

            constexpr static mftype::Type TYPE = mftype::PrimitiveType::INT16;

            std::string debug_repr() const override { return "type: int16"; }
        };

        /**
         * @brief Node reflecting the int 32-bit type.
         *
         * @inedge TODO
         */
        struct Int32Node : public TypeNode {
            Int32Node(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) :
                TypeNode(_first_token, _last_token) {
                // Empty.
            }

            virtual ~Int32Node() {
                // Empty.
            }

            constexpr static mftype::Type TYPE = mftype::PrimitiveType::INT32;

            std::string debug_repr() const override { return "type: int32"; }
        };

        /**
         * @brief Node reflecting the int 64-bit type.
         *
         * @inedge TODO
         */
        struct Int64Node : public TypeNode {
            Int64Node(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) :
                TypeNode(_first_token, _last_token) {
                // Empty.
            }

            virtual ~Int64Node() {
                // Empty.
            }

            constexpr static mftype::Type TYPE = mftype::PrimitiveType::INT64;

            std::string debug_repr() const override { return "type: int64"; }
        };

        /**
         * @brief Node reflecting the unsigned int type.
         *
         * @inedge TODO
         */
        struct UIntNode : public TypeNode {
            UIntNode(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) :
                TypeNode(_first_token, _last_token) {
                // Empty.
            }

            virtual ~UIntNode() {
                // Empty.
            }

            constexpr static mftype::Type TYPE = mftype::PrimitiveType::UINT;

            std::string debug_repr() const override { return "type: uint"; }
        };

        /**
         * @brief Node reflecting the unsigned int 8-bit type.
         *
         * @inedge TODO
         */
        struct UInt8Node : public TypeNode {
            UInt8Node(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) :
                TypeNode(_first_token, _last_token) {
                // Empty.
            }

            virtual ~UInt8Node() {
                // Empty.
            }

            constexpr static mftype::Type TYPE = mftype::PrimitiveType::UINT8;

            std::string debug_repr() const override { return "type: uint8"; }
        };

        /**
         * @brief Node reflecting the unsigned int 16-bit type.
         *
         * @inedge TODO
         */
        struct UInt16Node : public TypeNode {
            UInt16Node(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) :
                TypeNode(_first_token, _last_token) {
                // Empty.
            }

            virtual ~UInt16Node() {
                // Empty.
            }

            constexpr static mftype::Type TYPE = mftype::PrimitiveType::UINT16;

            std::string debug_repr() const override { return "type: uint16"; }
        };

        /**
         * @brief Node reflecting the unsigned int 32-bit type.
         *
         * @inedge TODO
         */
        struct UInt32Node : public TypeNode {
            UInt32Node(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) :
                TypeNode(_first_token, _last_token) {
                // Empty.
            }

            virtual ~UInt32Node() {
                // Empty.
            }

            constexpr static mftype::Type TYPE = mftype::PrimitiveType::UINT32;

            std::string debug_repr() const override { return "type: uint32"; }
        };

        /**
         * @brief Node reflecting the unsigned int 64-bit type.
         *
         * @inedge TODO
         */
        struct UInt64Node : public TypeNode {
            UInt64Node(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) :
                TypeNode(_first_token, _last_token) {
                // Empty.
            }

            virtual ~UInt64Node() {
                // Empty.
            }

            constexpr static mftype::Type TYPE = mftype::PrimitiveType::UINT64;

            std::string debug_repr() const override { return "type: uint64"; }
        };

        /**
         * @brief Node reflecting the float 64-bit type.
         *
         * @inedge TODO
         */
        struct Float32Node : public TypeNode {
            Float32Node(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) :
                TypeNode(_first_token, _last_token) {
                // Empty.
            }

            virtual ~Float32Node() {
                // Empty.
            }

            constexpr static mftype::Type TYPE = mftype::PrimitiveType::FLOAT32;

            std::string debug_repr() const override { return "type: float32"; }
        };

        /**
         * @brief Node reflecting the float 64-bit type.
         *
         * @inedge TODO
         */
        struct Float64Node : public TypeNode {
            Float64Node(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) :
                TypeNode(_first_token, _last_token) {
                // Empty.
            }

            virtual ~Float64Node() {
                // Empty.
            }

            constexpr static mftype::Type TYPE = mftype::PrimitiveType::FLOAT64;

            std::string debug_repr() const override { return "type: float64"; }
        };

        struct GetTypeVisitor {
            template <typename TypeNodeType>
            mftype::Type operator()(const TypeNodeType&) {
                if constexpr (std::is_base_of_v<TypeNode, TypeNodeType>) {
                    return TypeNodeType::TYPE;
                } else {
                    return mftype::UnresolvedType{};
                }
            }
        };
    }  // namespace ast
}  // namespace mattflow
namespace mfast = mattflow::ast;

#endif  // __mattflow_ast_node_type_h
