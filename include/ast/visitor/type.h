#ifndef __mattflow_ast_visitor_type_h
#define __mattflow_ast_visitor_type_h

#include "ast/node/type.h"
#include "variable/variable.h"

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
            mfvar::VariableTypeTable& var_table;
            mfvar::Scope              scope;

            template <typename NodeType>
            void operator()(const NodeType& target) {
                if constexpr (std::is_base_of_v<IdentifierNode, NodeType>) {
                    var_table.associate_type(scope, target.name, TypeNodeType::TYPE);
                } else if constexpr (std::is_base_of_v<ParenExprNode, NodeType>) {
                    // TODO(Matthew): do we want to do anything here?
                }
            }
        };
    }  // namespace ast
}  // namespace mattflow
namespace mfast = mattflow::ast;

#endif  // __mattflow_ast_visitor_type_h
