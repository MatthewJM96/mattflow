#ifndef __mattflow_ast_visitor_operator_h
#define __mattflow_ast_visitor_operator_h

#include "ast/node/operator.h"

namespace mattflow {
    namespace ast {
        struct GetOrderVisitor {
            template <typename OperatorNodeType>
            Order operator()(const OperatorNodeType&) {
                if constexpr (std::is_base_of_v<OperatorNode, OperatorNodeType>) {
                    return OperatorNodeType::ORDER;
                } else {
                    return Order::NONE;
                }
            }
        };

        struct GetPrecedenceVisitor {
            template <typename OperatorNodeType>
            Precedence operator()(const OperatorNodeType&) {
                if constexpr (std::is_base_of_v<OperatorNode, OperatorNodeType>) {
                    return OperatorNodeType::PRECEDENCE;
                } else {
                    return Precedence::NONE;
                }
            }
        };

        struct GetAssociativityVisitor {
            template <typename OperatorNodeType>
            Associativity operator()(const OperatorNodeType&) {
                if constexpr (std::is_base_of_v<OperatorNode, OperatorNodeType>) {
                    return OperatorNodeType::ASSOCIATIVITY;
                } else {
                    return Associativity::SENTINEL;
                }
            }
        };
    }  // namespace ast
}  // namespace mattflow
namespace mfast = mattflow::ast;

#endif  // __mattflow_ast_visitor_operator_h
