#include "stdafx.h"

#include "ast/link.h"
#include "ast/node.h"
#include "ast/single_token.h"
#include "ast/state.h"
#include "literal/identifier.h"

#include "ast/parse.h"

void mfast::parse(
    VALIN const mflex::Tokens& tokens,
    VALOUT AST&                ast,
    VALOUT NodeBuffers&        nodes,
    VALOUT mftype::IdentifierTypeTable& type_table
) {
    (void)type_table;

    // Ensure buffers are clear for building fresh AST.
    ast.clear();
    nodes.node_info.clear();
    nodes.vertex_node_map = boost::get(vertex_data, ast);

    // Prepare stack for precedence, associativity, and last attachable vertex with the
    // top-level block node for module.
    ParserState parser_state;
    push_enclosure(
        BlockExprNode{ tokens.begin(), tokens.end() },
        EnclosingCategory::ROOT,
        ast,
        nodes,
        parser_state
    );

    // Iterate tokens, parsing as appropriate.
    auto it = tokens.begin();
    while (it != tokens.end()) {
        switch (it->type) {
            case mflex::TokenType::IF:
            case mflex::TokenType::THEN:
            case mflex::TokenType::ELIF:
            case mflex::TokenType::ELSE:
            case mflex::TokenType::FOR:
            case mflex::TokenType::IN:
            case mflex::TokenType::WHERE:
            case mflex::TokenType::WHILE:
            case mflex::TokenType::DO:
            case mflex::TokenType::MATCH:
            case mflex::TokenType::PRINT:
            case mflex::TokenType::STRUCT:
            case mflex::TokenType::LEFT_BRACKET:
                //     // TODO(Matthew): ambiguity as could be number range or list.
                //     //                  need a way to decide which we are seeing
                //     here.
                //     //                  are we happy with this approach?
                //     if ((it + 2)->type == mflex::TokenType::RANGE) {
                //         // Add range vertex, push precedence parser_expects::PRIMARY.
                //         add_number_range_node(it, ast, nodes, parser_state);
                //     } else {
                //         // Add list vertex, push precedence
                //         parser_expects::EXPRESSION. add_list_node(it, ast, nodes,
                //         parser_state);
                //     }
                //     continue;
            case mflex::TokenType::RIGHT_BRACKET:
            case mflex::TokenType::COMMA:
            case mflex::TokenType::RANGE:
            case mflex::TokenType::DOT:
            case mflex::TokenType::ARROW:
            case mflex::TokenType::SENTINEL:
                break;
            case mflex::TokenType::LEFT_BRACE:
                // Link operations on stack if we have an end of expression.
                mfast::maybe_link_operations_on_stack(ast, nodes, parser_state);

                // Push a new enclosure for block expr.
                push_enclosure(
                    BlockExprNode{ it, it },
                    EnclosingCategory::BLOCK,
                    ast,
                    nodes,
                    parser_state
                );

                // Move forward a token.
                it += 1;
                continue;
            case mflex::TokenType::RIGHT_BRACE:
                // Pop enclosure of block expr.
                pop_enclosure(
                    mfast::EnclosingCategory::BLOCK, ast, nodes, parser_state
                );

                // Move forward a token.
                it += 1;
                continue;
            case mflex::TokenType::LEFT_PAREN:
                // Link operations on stack if we have an end of expression.
                mfast::maybe_link_operations_on_stack(ast, nodes, parser_state);

                // Push a new enclosure for paren expr.
                push_enclosure(
                    ParenExprNode{ it, it },
                    EnclosingCategory::PARENTHESES,
                    ast,
                    nodes,
                    parser_state
                );

                // Move forward a token.
                it += 1;
                continue;
            case mflex::TokenType::RIGHT_PAREN:
                // Pop enclosure of paren expr.
                pop_enclosure(
                    mfast::EnclosingCategory::PARENTHESES, ast, nodes, parser_state
                );

                // Move forward a token.
                it += 1;
                continue;
            case mflex::TokenType::ASSIGN_TYPE:
                // Add ASSIGN_TYPE vertex.
                add_single_token_op<mfast::AssignTypeOperatorNode>(
                    it, ast, nodes, parser_state
                );
                continue;
            case mflex::TokenType::ASSIGN_VALUE:
                // Add ASSIGN_VALUE vertex.
                add_single_token_op<mfast::AssignValueOperatorNode>(
                    it, ast, nodes, parser_state
                );
                continue;
            case mflex::TokenType::AND:
                // Add AND vertex.
                add_single_token_op<mfast::AndOperatorNode>(
                    it, ast, nodes, parser_state
                );
                continue;
            case mflex::TokenType::OR:
                // Add OR vertex.
                add_single_token_op<mfast::OrOperatorNode>(
                    it, ast, nodes, parser_state
                );
                continue;
            case mflex::TokenType::EQUALS:
                // Add EQUALS vertex.
                add_single_token_op<mfast::EqualOperatorNode>(
                    it, ast, nodes, parser_state
                );
                continue;
            case mflex::TokenType::NOT_EQUALS:
                // Add NOT EQUALS vertex.
                add_single_token_op<mfast::NotEqualOperatorNode>(
                    it, ast, nodes, parser_state
                );
                continue;
            case mflex::TokenType::LESS_THAN:
                // Add LESS THAN vertex.
                add_single_token_op<mfast::LesserOperatorNode>(
                    it, ast, nodes, parser_state
                );
                continue;
            case mflex::TokenType::LESS_THAN_OR_EQUAL_TO:
                // Add LESS THAN OR EQUAL TO vertex.
                add_single_token_op<mfast::LesserOrEqualOperatorNode>(
                    it, ast, nodes, parser_state
                );
                continue;
            case mflex::TokenType::GREATER_THAN:
                // Add GREATER THAN vertex.
                add_single_token_op<mfast::GreaterOperatorNode>(
                    it, ast, nodes, parser_state
                );
                continue;
            case mflex::TokenType::GREATER_THAN_OR_EQUAL_TO:
                // Add GREATER THAN OR EQUAL TO vertex.
                add_single_token_op<mfast::GreaterOrEqualOperatorNode>(
                    it,

                    ast,
                    nodes,
                    parser_state
                );
                continue;
            case mflex::TokenType::PLUS:
                // Add ADDITION vertex.
                add_single_token_op<mfast::AdditionOperatorNode>(
                    it, ast, nodes, parser_state
                );
                continue;
            case mflex::TokenType::MINUS:
                // TODO(Matthew): for now this token can be both repr of unary and
                //                binary operations (subtraction and negation).
                // Add SUBTRACTION vertex.
                add_single_token_op<mfast::SubtractionOperatorNode>(
                    it,

                    ast,
                    nodes,
                    parser_state
                );
                continue;
            case mflex::TokenType::SLASH:
                // Add DIVIDE vertex.
                add_single_token_op<mfast::DivisionOperatorNode>(
                    it, ast, nodes, parser_state
                );
                continue;
            case mflex::TokenType::STAR:
                // Add MULTIPLY vertex.
                add_single_token_op<mfast::MultiplicationOperatorNode>(
                    it, ast, nodes, parser_state
                );
                continue;
            case mflex::TokenType::POWER:
                // Add POWER vertex.
                add_single_token_op<mfast::PowerOperatorNode>(
                    it, ast, nodes, parser_state
                );
                continue;
            case mflex::TokenType::NOT:
                // Add NOT vertex.
                add_single_token_op<mfast::NotOperatorNode>(
                    it, ast, nodes, parser_state
                );
                continue;
            case mflex::TokenType::NIL:
                // Add null vertex.
                add_single_token_nonop<mfast::NullNode>(it, ast, nodes, parser_state);
                continue;
            case mflex::TokenType::CHAR:
                // Add CHAR vertex.
                add_single_token_nonop<mfast::CharNode>(it, ast, nodes, parser_state);
                continue;
            case mflex::TokenType::BOOL:
                // Add BOOL vertex.
                add_single_token_nonop<mfast::BoolNode>(it, ast, nodes, parser_state);
                continue;
            case mflex::TokenType::INT:
                // Add INT vertex.
                add_single_token_nonop<mfast::IntNode>(it, ast, nodes, parser_state);
                continue;
            case mflex::TokenType::INT8:
                // Add INT8 vertex.
                add_single_token_nonop<mfast::Int8Node>(it, ast, nodes, parser_state);
                continue;
            case mflex::TokenType::INT16:
                // Add INT16 vertex.
                add_single_token_nonop<mfast::Int16Node>(it, ast, nodes, parser_state);
                continue;
            case mflex::TokenType::INT32:
                // Add INT32 vertex.
                add_single_token_nonop<mfast::Int32Node>(it, ast, nodes, parser_state);
                continue;
            case mflex::TokenType::INT64:
                // Add INT64 vertex.
                add_single_token_nonop<mfast::Int64Node>(it, ast, nodes, parser_state);
                continue;
            case mflex::TokenType::UINT:
                // Add UINT vertex.
                add_single_token_nonop<mfast::UIntNode>(it, ast, nodes, parser_state);
                continue;
            case mflex::TokenType::UINT8:
                // Add UINT8 vertex.
                add_single_token_nonop<mfast::UInt8Node>(it, ast, nodes, parser_state);
                continue;
            case mflex::TokenType::UINT16:
                // Add UINT16 vertex.
                add_single_token_nonop<mfast::UInt16Node>(it, ast, nodes, parser_state);
                continue;
            case mflex::TokenType::UINT32:
                // Add UINT32 vertex.
                add_single_token_nonop<mfast::UInt32Node>(it, ast, nodes, parser_state);
                continue;
            case mflex::TokenType::UINT64:
                // Add UINT64 vertex.
                add_single_token_nonop<mfast::UInt64Node>(it, ast, nodes, parser_state);
                continue;
            case mflex::TokenType::FLOAT32:
                // Add FP32 vertex.
                add_single_token_nonop<mfast::Float32Node>(
                    it, ast, nodes, parser_state
                );
                continue;
            case mflex::TokenType::FLOAT64:
                // Add FP64 vertex.
                add_single_token_nonop<mfast::Float64Node>(
                    it, ast, nodes, parser_state
                );
                continue;
            case mflex::TokenType::TRUE:
            case mflex::TokenType::FALSE:
                // Add Boolean value vertex.
                add_single_token_nonop<mfast::BoolValNode>(
                    it, ast, nodes, parser_state, it->type == mflex::TokenType::TRUE
                );
                continue;
            case mflex::TokenType::NUMBER:
                // Add number vertex.
                add_single_token_nonop<mfast::NumberValNode>(
                    it, ast, nodes, parser_state, it->number
                );
                continue;
            case mflex::TokenType::STRING:
                // Add string vertex.
                add_single_token_nonop<mfast::StringValNode>(
                    it, ast, nodes, parser_state, it->string_idx
                );
                continue;
            case mflex::TokenType::IDENTIFIER:
                // Add identifier vertex.
                add_single_token_nonop<mfast::IdentifierNode>(
                    it, ast, nodes, parser_state, it->identifier_idx
                );
                continue;
        }
    }

    // Pop root enclosure.
    pop_enclosure(EnclosingCategory::ROOT, ast, nodes, parser_state);
}
