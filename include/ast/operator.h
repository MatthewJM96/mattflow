#ifndef __mattflow_ast_operator_h
#define __mattflow_ast_operator_h

#include "ast/ast.h"
#include "ast/node.h"

namespace mattflow {
    namespace ast {
        struct GetOrderVisitor {
            Order result;

            template <typename OperatorNodeType>
            void operator()(const OperatorNodeType& node_info) {
                if constexpr (std::is_base_of_v<OperatorNode, OperatorNodeType>) {
                    result = node_info.ORDER;
                }
            }
        };

        struct GetPrecedenceVisitor {
            Precedence result;

            template <typename OperatorNodeType>
            void operator()(const OperatorNodeType& node_info) {
                if constexpr (std::is_base_of_v<OperatorNode, OperatorNodeType>) {
                    result = node_info.PRECEDENCE;
                }
            }
        };

        struct GetAssociativityVisitor {
            Associativity result;

            template <typename OperatorNodeType>
            void operator()(const OperatorNodeType& node_info) {
                if constexpr (std::is_base_of_v<OperatorNode, OperatorNodeType>) {
                    result = node_info.ASSOCIATIVITY;
                }
            }
        };
    }  // namespace ast
}  // namespace mattflow
namespace mfast = mattflow::ast;

#endif  // __mattflow_ast_operator_h
