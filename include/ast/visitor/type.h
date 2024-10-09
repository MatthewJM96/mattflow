#ifndef __mattflow_ast_visitor_type_h
#define __mattflow_ast_visitor_type_h

#include "ast/node/type.h"

namespace mattflow {
    namespace ast {
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

        template <typename TypeNodeType>
            requires (std::is_base_of_v<TypeNode, TypeNodeType>)
        struct AssignTypeVisitor {
            mftype::IdentifierTypeTable& type_table;

            template <typename NodeType>
            void operator()(const NodeType& target) {
                if constexpr (std::is_base_of_v<IdentifierNode, NodeType>) {
                    type_table.associate_type(target->name, NodeType::TYPE);
                } else if constexpr (std::is_base_of_v<ParenExprNode, NodeType>) {
                    //
                }
            }
        };
    }  // namespace ast
}  // namespace mattflow
namespace mfast = mattflow::ast;

#endif  // __mattflow_ast_visitor_type_h
