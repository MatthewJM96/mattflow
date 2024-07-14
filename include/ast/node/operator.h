#ifndef __mattflow_ast_node_operator_h
#define __mattflow_ast_node_operator_h

#include "ast/node/node.h"
#include "lex/token_type.h"

namespace mattflow {
    namespace ast {
        enum class OperatorKind : int16_t {
            // Boolean operators.
            NOT        = static_cast<int16_t>(mflex::TokenType::NOT),
            EQUALS     = static_cast<int16_t>(mflex::TokenType::EQUALS),
            NOT_EQUALS = static_cast<int16_t>(mflex::TokenType::NOT_EQUALS),
            LESS_THAN  = static_cast<int16_t>(mflex::TokenType::LESS_THAN),
            LESS_THAN_OR_EQUAL_TO
                = static_cast<int16_t>(mflex::TokenType::LESS_THAN_OR_EQUAL_TO),
            GREATER_THAN = static_cast<int16_t>(mflex::TokenType::GREATER_THAN),
            GREATER_THAN_OR_EQUAL_TO
                = static_cast<int16_t>(mflex::TokenType::GREATER_THAN_OR_EQUAL_TO),

            // Assignment.
            ASSIGN_VALUE = static_cast<int16_t>(mflex::TokenType::ASSIGN_VALUE),

            // Arithmetic operators.
            MINUS = static_cast<int16_t>(mflex::TokenType::MINUS),
            PLUS  = static_cast<int16_t>(mflex::TokenType::PLUS),
            SLASH = static_cast<int16_t>(mflex::TokenType::SLASH),
            STAR  = static_cast<int16_t>(mflex::TokenType::STAR)
        };

        OperatorKind cast_token_to_intrinsic(mflex::TokenType tok) {
            assert(
                static_cast<int16_t>(tok) >= static_cast<int16_t>(mflex::TokenType::NOT)
            );
            assert(
                static_cast<int16_t>(tok)
                <= static_cast<int16_t>(mflex::TokenType::STAR)
            );
            assert(
                static_cast<int16_t>(tok)
                != static_cast<int16_t>(mflex::TokenType::ASSIGN_TYPE)
            );

            return static_cast<OperatorKind>(static_cast<int16_t>(tok));
        }

        /**
         * @brief Node reflecting an operator.
         *
         * @inedge operator | paren expression | assignment | block expression
         * @outedges rvalue expression
         */
        struct OperatorNode : public Node {
            OperatorKind kind;
        };
    }  // namespace ast
}  // namespace mattflow
namespace mfast = mattflow::ast;

#endif  // __mattflow_ast_node_operator_h
