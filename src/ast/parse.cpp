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
    // TODO(Matthew): make sure tokens isn't silly length.
    nodes.node_info.reserve(tokens.size());
    nodes.vertex_node_map = boost::get(vertex_data, ast);

    // Prepare stack for precedence, associativity, and last attachable vertex with the
    // top-level block node for module.
    ParserState parser_state;
    push_enclosure(
        BlockExprNode{ tokens.begin(), tokens.end() },
        EnclosingProps::ROOT | EnclosingProps::MULTI_EXPR,
        ast,
        nodes,
        parser_state
    );

    // Iterate tokens, parsing as appropriate.
    auto it = tokens.begin();
    while (it != tokens.end()) {
        switch (it->type) {
            case mflex::TokenType::IF:
                // Link operations on stack if we have an end of expression.
                maybe_link_operations_on_stack(ast, nodes, parser_state);

                // Push a new enclosure for paren expr.
                push_enclosure(
                    IfExprNode{ it, it },
                    EnclosingProps::MULTI_EXPR | EnclosingProps::IF,
                    ast,
                    nodes,
                    parser_state
                );

                parser_state.last_seen.back() = NodeCategory::IF;

                // Move forward a token.
                it += 1;
                continue;
            case mflex::TokenType::THEN:
                mfassert(
                    (parser_state.enclosed_by.back() & EnclosingProps::IF)
                        == EnclosingProps::IF,
                    "Encountered a 'then' but not in an if-statement."
                );

                // TODO(Matthew): This won't be true as the expression between the 'if'
                //                (or 'elif') and the 'then' will change the last_seen
                //                value. How shall we validate this? Now is the last
                //                chance.
                mfassert(
                    parser_state.last_seen.back() == NodeCategory::IF
                        || parser_state.last_seen.back() == NodeCategory::ELIF,
                    "Encountered a 'then' which was not preceded by an 'if' or 'elif' "
                    "condition."
                );

                // Link operations on stack as we have an end of expression.
                // TODO(Matthew): detect failure case for this (aka unterminated
                //                expression).
                mfast::link_operations_on_stack(
                    mfast::Precedence::NONE, ast, nodes, parser_state
                );

                parser_state.last_seen.back() = NodeCategory::THEN;

                // Move forward a token.
                it += 1;
                continue;
            case mflex::TokenType::ELIF:
                mfassert(
                    (parser_state.enclosed_by.back() & EnclosingProps::IF)
                        == EnclosingProps::IF,
                    "Encountered a 'elif' but not in an if-statement."
                );

                mfassert(
                    parser_state.last_seen.back() == NodeCategory::THEN,
                    "Encountered an 'elif' which was not preceded by a 'then' "
                    "expression."
                );

                // Link operations on stack as we have an end of expression.
                // TODO(Matthew): detect failure case for this (aka unterminated
                //                expression).
                mfast::link_operations_on_stack(
                    mfast::Precedence::NONE, ast, nodes, parser_state
                );

                parser_state.last_seen.back() = NodeCategory::ELIF;

                // Move forward a token.
                it += 1;
                continue;
            case mflex::TokenType::ELSE:
                mfassert(
                    (parser_state.enclosed_by.back() & EnclosingProps::IF)
                        == EnclosingProps::IF,
                    "Encountered a 'else' but not in an if-statement."
                );

                mfassert(
                    parser_state.last_seen.back() == NodeCategory::THEN,
                    "Encountered an 'else' which was not preceded by a 'then' "
                    "expression."
                );

                parser_state.last_seen.back() = NodeCategory::ELSE;

                // Link operations on stack as we have an end of expression.
                // TODO(Matthew): detect failure case for this (aka unterminated
                //                expression).
                mfast::link_operations_on_stack(
                    mfast::Precedence::NONE, ast, nodes, parser_state
                );

                // Move forward a token.
                it += 1;
                continue;
            case mflex::TokenType::FOR:
            case mflex::TokenType::IN:
            case mflex::TokenType::WHERE:
            case mflex::TokenType::WHILE:
            case mflex::TokenType::DO:
            case mflex::TokenType::MATCH:
            case mflex::TokenType::PRINT:
            case mflex::TokenType::ARROW:
            case mflex::TokenType::SENTINEL:
                break;
            case mflex::TokenType::LEFT_BRACKET:
                // Link operations on stack if we have an end of expression.
                maybe_link_operations_on_stack(ast, nodes, parser_state);

                // TODO(Matthew): We can differentiate between number range and list
                //                once the AST exists, as the first operator descending
                //                into the enclosure will either be "," or "..".

                // Push a new enclosure for paren expr.
                push_enclosure(
                    ParenExprNode{ it, it },
                    EnclosingProps::SINGLE_EXPR,
                    ast,
                    nodes,
                    parser_state
                );

                // Move forward a token.
                it += 1;
                continue;
            case mflex::TokenType::RIGHT_BRACKET:
                // Pop enclosure of paren expr.
                pop_enclosure(EnclosingProps::SINGLE_EXPR, ast, nodes, parser_state);

                // Move forward a token.
                it += 1;
                continue;
            case mflex::TokenType::STRUCT:
                mfassert(
                    (it + 1)->type == mflex::TokenType::LEFT_BRACE,
                    "Saw struct keyword, expected it to be followed by '{'."
                );

                // TODO(Matthew): we don't do anything to prevent operations acting on
                //                types. Maybe we want to allow this, but if so we still
                //                want to avoid things like:
                //                  struct { ... } * 2
                //                maybe we check later.

                // Link operations on stack if we have an end of expression.
                maybe_link_operations_on_stack(ast, nodes, parser_state);

                // TODO(Matthew): we haven't done anything to explicitly add logic about
                //                StructFieldNode vertices. Maybe we'll add these later.

                // TODO(Matthew): right now we set type to nullptr as struct type
                //                definitions are anonymous, only nameable by assigning
                //                to an identifier.

                // Push a new enclosure for block expr.
                push_enclosure(
                    StructNode{ it, it, nullptr },
                    EnclosingProps::STRUCT | EnclosingProps::MULTI_EXPR,
                    ast,
                    nodes,
                    parser_state
                );

                // Move forward two tokens (one extra for '{').
                it += 2;
                continue;
            case mflex::TokenType::LEFT_BRACE:
                // Link operations on stack if we have an end of expression.
                maybe_link_operations_on_stack(ast, nodes, parser_state);

                // Push a new enclosure for block expr.
                push_enclosure(
                    BlockExprNode{ it, it },
                    EnclosingProps::MULTI_EXPR,
                    ast,
                    nodes,
                    parser_state
                );

                // Move forward a token.
                it += 1;
                continue;
            case mflex::TokenType::RIGHT_BRACE:
                // Pop enclosure of block expr.
                pop_enclosure(EnclosingProps::MULTI_EXPR, ast, nodes, parser_state);

                // Move forward a token.
                it += 1;
                continue;
            case mflex::TokenType::LEFT_PAREN:
                // Link operations on stack if we have an end of expression.
                maybe_link_operations_on_stack(ast, nodes, parser_state);

                // Push a new enclosure for paren expr.
                push_enclosure(
                    ParenExprNode{ it, it },
                    EnclosingProps::SINGLE_EXPR,
                    ast,
                    nodes,
                    parser_state
                );

                // Move forward a token.
                it += 1;
                continue;
            case mflex::TokenType::RIGHT_PAREN:
                // Pop enclosure of paren expr.
                pop_enclosure(EnclosingProps::SINGLE_EXPR, ast, nodes, parser_state);

                // Move forward a token.
                it += 1;
                continue;
            case mflex::TokenType::ASSIGN_TYPE:
                // Add ASSIGN_TYPE vertex.
                add_single_token_op<AssignTypeOperatorNode>(
                    it, ast, nodes, parser_state
                );
                continue;
            case mflex::TokenType::ASSIGN_VALUE:
                // Add ASSIGN_VALUE vertex.
                add_single_token_op<AssignValueOperatorNode>(
                    it, ast, nodes, parser_state
                );
                continue;
            case mflex::TokenType::COMMA:
                // Add COMMA vertex.
                add_single_token_op<CommaOperatorNode>(it, ast, nodes, parser_state);
                continue;
            case mflex::TokenType::RANGE:
                // Add RANGE vertex.
                add_single_token_op<RangeOperatorNode>(it, ast, nodes, parser_state);
                continue;
            case mflex::TokenType::AND:
                // Add AND vertex.
                add_single_token_op<AndOperatorNode>(it, ast, nodes, parser_state);
                continue;
            case mflex::TokenType::OR:
                // Add OR vertex.
                add_single_token_op<OrOperatorNode>(it, ast, nodes, parser_state);
                continue;
            case mflex::TokenType::EQUALS:
                // Add EQUALS vertex.
                add_single_token_op<EqualOperatorNode>(it, ast, nodes, parser_state);
                continue;
            case mflex::TokenType::NOT_EQUALS:
                // Add NOT EQUALS vertex.
                add_single_token_op<NotEqualOperatorNode>(it, ast, nodes, parser_state);
                continue;
            case mflex::TokenType::LESS_THAN:
                // Add LESS THAN vertex.
                add_single_token_op<LesserOperatorNode>(it, ast, nodes, parser_state);
                continue;
            case mflex::TokenType::LESS_THAN_OR_EQUAL_TO:
                // Add LESS THAN OR EQUAL TO vertex.
                add_single_token_op<LesserOrEqualOperatorNode>(
                    it, ast, nodes, parser_state
                );
                continue;
            case mflex::TokenType::GREATER_THAN:
                // Add GREATER THAN vertex.
                add_single_token_op<GreaterOperatorNode>(it, ast, nodes, parser_state);
                continue;
            case mflex::TokenType::GREATER_THAN_OR_EQUAL_TO:
                // Add GREATER THAN OR EQUAL TO vertex.
                add_single_token_op<GreaterOrEqualOperatorNode>(
                    it,

                    ast,
                    nodes,
                    parser_state
                );
                continue;
            case mflex::TokenType::PLUS:
                // Add ADDITION vertex.
                add_single_token_op<AdditionOperatorNode>(it, ast, nodes, parser_state);
                continue;
            case mflex::TokenType::MINUS:
                // TODO(Matthew): for now this token can be both repr of unary and
                //                binary operations (subtraction and negation).
                // Add SUBTRACTION vertex.
                add_single_token_op<SubtractionOperatorNode>(
                    it,

                    ast,
                    nodes,
                    parser_state
                );
                continue;
            case mflex::TokenType::SLASH:
                // Add DIVIDE vertex.
                add_single_token_op<DivisionOperatorNode>(it, ast, nodes, parser_state);
                continue;
            case mflex::TokenType::STAR:
                // Add MULTIPLY vertex.
                add_single_token_op<MultiplicationOperatorNode>(
                    it, ast, nodes, parser_state
                );
                continue;
            case mflex::TokenType::POWER:
                // Add POWER vertex.
                add_single_token_op<PowerOperatorNode>(it, ast, nodes, parser_state);
                continue;
            case mflex::TokenType::NOT:
                // Add NOT vertex.
                add_single_token_op<NotOperatorNode>(it, ast, nodes, parser_state);
                continue;
            case mflex::TokenType::DOT:
                // Add DOT vertex.
                add_single_token_op<DotOperatorNode>(it, ast, nodes, parser_state);
                continue;
            case mflex::TokenType::NIL:
                // Add null vertex.
                add_single_token_nonop<NullNode>(it, ast, nodes, parser_state);
                continue;
            case mflex::TokenType::CHAR:
                // Add CHAR vertex.
                add_single_token_nonop<CharNode>(it, ast, nodes, parser_state);
                continue;
            case mflex::TokenType::BOOL:
                // Add BOOL vertex.
                add_single_token_nonop<BoolNode>(it, ast, nodes, parser_state);
                continue;
            case mflex::TokenType::INT:
                // Add INT vertex.
                add_single_token_nonop<IntNode>(it, ast, nodes, parser_state);
                continue;
            case mflex::TokenType::INT8:
                // Add INT8 vertex.
                add_single_token_nonop<Int8Node>(it, ast, nodes, parser_state);
                continue;
            case mflex::TokenType::INT16:
                // Add INT16 vertex.
                add_single_token_nonop<Int16Node>(it, ast, nodes, parser_state);
                continue;
            case mflex::TokenType::INT32:
                // Add INT32 vertex.
                add_single_token_nonop<Int32Node>(it, ast, nodes, parser_state);
                continue;
            case mflex::TokenType::INT64:
                // Add INT64 vertex.
                add_single_token_nonop<Int64Node>(it, ast, nodes, parser_state);
                continue;
            case mflex::TokenType::UINT:
                // Add UINT vertex.
                add_single_token_nonop<UIntNode>(it, ast, nodes, parser_state);
                continue;
            case mflex::TokenType::UINT8:
                // Add UINT8 vertex.
                add_single_token_nonop<UInt8Node>(it, ast, nodes, parser_state);
                continue;
            case mflex::TokenType::UINT16:
                // Add UINT16 vertex.
                add_single_token_nonop<UInt16Node>(it, ast, nodes, parser_state);
                continue;
            case mflex::TokenType::UINT32:
                // Add UINT32 vertex.
                add_single_token_nonop<UInt32Node>(it, ast, nodes, parser_state);
                continue;
            case mflex::TokenType::UINT64:
                // Add UINT64 vertex.
                add_single_token_nonop<UInt64Node>(it, ast, nodes, parser_state);
                continue;
            case mflex::TokenType::FLOAT32:
                // Add FP32 vertex.
                add_single_token_nonop<Float32Node>(it, ast, nodes, parser_state);
                continue;
            case mflex::TokenType::FLOAT64:
                // Add FP64 vertex.
                add_single_token_nonop<Float64Node>(it, ast, nodes, parser_state);
                continue;
            case mflex::TokenType::TRUE:
            case mflex::TokenType::FALSE:
                // Add Boolean value vertex.
                add_single_token_nonop<BoolValNode>(
                    it, ast, nodes, parser_state, it->type == mflex::TokenType::TRUE
                );
                continue;
            case mflex::TokenType::NUMBER:
                // Add number vertex.
                add_single_token_nonop<NumberValNode>(
                    it, ast, nodes, parser_state, it->number
                );
                continue;
            case mflex::TokenType::STRING:
                // Add string vertex.
                add_single_token_nonop<StringValNode>(
                    it, ast, nodes, parser_state, it->string_idx
                );
                continue;
            case mflex::TokenType::IDENTIFIER:
                // Add identifier vertex.
                add_single_token_nonop<IdentifierNode>(
                    it, ast, nodes, parser_state, it->identifier_idx
                );
                continue;
        }
    }

    // May have an unpopped if-expression.
    if ((parser_state.enclosed_by.back() & EnclosingProps::IF) == EnclosingProps::IF) {
        pop_enclosure(EnclosingProps::IF, ast, nodes, parser_state);
    }

    // Pop root enclosure.
    pop_enclosure(
        EnclosingProps::ROOT | EnclosingProps::MULTI_EXPR, ast, nodes, parser_state
    );
}
