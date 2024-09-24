#ifndef __mattflow_ast_operator_h
#define __mattflow_ast_operator_h

#include "ast/ast.h"
#include "ast/node.h"

namespace mattflow {
    namespace ast {
        struct GetOrderVisitor {
            template <typename OperatorNodeType>
            Order operator()(const OperatorNodeType& node_info) {
                if constexpr (std::is_base_of_v<OperatorNode, OperatorNodeType>) {
                    return node_info.ORDER;
                } else {
                    return Order::NONE;
                }
            }
        };

        struct GetPrecedenceVisitor {
            template <typename OperatorNodeType>
            Precedence operator()(const OperatorNodeType& node_info) {
                if constexpr (std::is_base_of_v<OperatorNode, OperatorNodeType>) {
                    return node_info.PRECEDENCE;
                } else {
                    return Precedence::NONE;
                }
            }
        };

        struct GetAssociativityVisitor {
            template <typename OperatorNodeType>
            Associativity operator()(const OperatorNodeType& node_info) {
                if constexpr (std::is_base_of_v<OperatorNode, OperatorNodeType>) {
                    return node_info.ASSOCIATIVITY;
                } else {
                    return Associativity::SENTINEL;
                }
            }
        };
    }  // namespace ast
}  // namespace mattflow
namespace mfast = mattflow::ast;

#endif  // __mattflow_ast_operator_h
