#include "stdafx.h"

#include "ast/node.h"
#include "literal/identifier.h"

#include "ast/parse.h"

bool maybe_see_type(VALIN const mflex::Tokens::const_iterator& token) {
    return token->type == mflex::TokenType::IDENTIFIER
           || token->type == mflex::TokenType::STRUCT
           || token->type == mflex::TokenType::LEFT_PAREN;
}

void add_number_node(
    VALINOUT mflex::Tokens::const_iterator& curr_token,
    VALOUT mfast::AST& ast,
    VALOUT mfast::NodeBuffers& nodes,
    VALOUT mfast::ParserState& parser_state
) {
    // No validation needed here, the current token will be a number token which is
    // assured to be valid during lexing.
    assert(curr_token->type == mflex::TokenType::NUMBER);

    nodes.numbers.emplace_back(mfast::NumberNode{
        curr_token, curr_token, curr_token->number });

    auto vertex      = boost::add_vertex(ast);
    auto prev_vertex = parser_state.vertex[parser_state.cursor];
    parser_state.vertex[++parser_state.cursor] = vertex;
    nodes.vertex_node_map[vertex]              = &nodes.numbers.back();

    boost::add_edge(prev_vertex, vertex, ast);

    // number
    curr_token += 1;
}

void add_string_node(
    VALINOUT mflex::Tokens::const_iterator& curr_token,
    VALOUT mfast::AST& ast,
    VALOUT mfast::NodeBuffers& nodes,
    VALOUT mfast::ParserState& parser_state
) {
    // No validation needed here, the current token will be a string token which is
    // assured to be valid during lexing.
    assert(curr_token->type == mflex::TokenType::STRING);

    nodes.strings.emplace_back(mfast::StringNode{
        curr_token, curr_token, std::move(curr_token->string) });

    auto vertex      = boost::add_vertex(ast);
    auto prev_vertex = parser_state.vertex[parser_state.cursor];
    parser_state.vertex[++parser_state.cursor] = vertex;
    nodes.vertex_node_map[vertex]              = &nodes.strings.back();

    boost::add_edge(prev_vertex, vertex, ast);

    // string
    curr_token += 1;
}

void add_variable_declaration_node(
    VALINOUT mflex::Tokens::const_iterator& curr_token,
    VALOUT mfast::AST& ast,
    VALOUT mfast::NodeBuffers& nodes,
    VALOUT mfast::ParserState& parser_state,
    VALOUT mfast::VariableTable& variable_table
) {
    // No validation needed here, the current token will be an identifier token which is
    // assured to be valid during lexing, and the type assignment character ":".
    assert(curr_token->type == mflex::TokenType::IDENTIFIER);
    assert((curr_token + 1)->type == mflex::TokenType::ASSIGN_TYPE);

    // TODO(Matthew): this will need revisiting to deal with scope.
    if (variable_table.contains(curr_token->identifier_idx)) {
        auto& ident = mflit::IdentifierTable::get();
        std::cout << "Variable " << ident.get(curr_token->identifier_idx)
                  << " already declared." << std::endl;
        exit(456);
    }

    // Mark variable as existing, next token should assign type and if it doesn't we
    // have an issue.
    variable_table[curr_token->identifier_idx] = nullptr;

    nodes.variables.emplace_back(mfast::VariableNode{
        curr_token, curr_token, curr_token->identifier_idx });

    auto vertex      = boost::add_vertex(ast);
    auto prev_vertex = parser_state.vertex[parser_state.cursor];
    parser_state.vertex[++parser_state.cursor] = vertex;
    nodes.vertex_node_map[vertex]              = &nodes.variables.back();

    boost::add_edge(prev_vertex, vertex, ast);

    // identifier ":"
    curr_token += 2;
}

void add_assignment_node(
    VALINOUT mflex::Tokens::const_iterator& curr_token,
    VALOUT mfast::AST& ast,
    VALOUT mfast::NodeBuffers& nodes,
    VALOUT mfast::ParserState& parser_state,
    VALOUT mfast::VariableTable& variable_table
) {
    // No validation needed here, the current token will be the value assignment
    // character "=".
    assert(curr_token->type == mflex::TokenType::ASSIGN_VALUE);

    nodes.assignments.emplace_back(mfast::AssignmentNode{ curr_token, curr_token });

    auto vertex      = boost::add_vertex(ast);
    auto prev_vertex = parser_state.vertex[parser_state.cursor];
    parser_state.vertex[++parser_state.cursor] = vertex;
    nodes.vertex_node_map[vertex]              = &nodes.assignments.back();

    boost::add_edge(prev_vertex, vertex, ast);

    // "="
    curr_token += 1;
}

void add_struct_node(
    VALINOUT mflex::Tokens::const_iterator& curr_token,
    VALOUT mfast::AST& ast,
    VALOUT mfast::NodeBuffers& nodes,
    VALOUT mfast::ParserState& parser_state,
    VALOUT mfast::VariableTable& variable_table,
    VALINOUT mftype::TypeTable& type_table
) {
    // No validation needed here, the current token will be a string token which is
    // assured to be valid during lexing.
    assert(curr_token->type == mflex::TokenType::STRUCT);
    assert((curr_token + 1)->type == mflex::TokenType::LEFT_BRACE);

    nodes.structs.emplace_back(mfast::StructNode{
        curr_token, curr_token, type_table.register_type_from_token(*curr_token) });

    auto vertex      = boost::add_vertex(ast);
    auto prev_vertex = parser_state.vertex[parser_state.cursor];
    parser_state.vertex[++parser_state.cursor] = vertex;
    nodes.vertex_node_map[vertex]              = &nodes.structs.back();

    boost::add_edge(prev_vertex, vertex, ast);

    // "struct" "{"
    curr_token += 2;
}

void add_struct_field_node(
    VALINOUT mflex::Tokens::const_iterator& curr_token,
    VALOUT mfast::AST& ast,
    VALOUT mfast::NodeBuffers& nodes,
    VALOUT mfast::ParserState& parser_state,
    VALOUT mfast::VariableTable& variable_table,
    VALINOUT mftype::TypeTable& type_table
) {
    // TODO(Matthew): implement correctly... just copied add_struct_node.

    // No validation needed here, the current token will be a string token which is
    // assured to be valid during lexing.
    assert(curr_token->type == mflex::TokenType::IDENTIFIER);
    assert((curr_token + 1)->type == mflex::TokenType::LEFT_BRACE);

    nodes.struct_fields.emplace_back(mfast::StructFieldNode{
        curr_token, curr_token, type_table.register_type_from_token(*curr_token) });

    auto vertex      = boost::add_vertex(ast);
    auto prev_vertex = parser_state.vertex[parser_state.cursor];
    parser_state.vertex[++parser_state.cursor] = vertex;
    nodes.vertex_node_map[vertex]              = &nodes.strings.back();

    boost::add_edge(prev_vertex, vertex, ast);

    // "struct" "{"
    curr_token += 2;
}

// TODO(Matthew): do we want type nodes? it seems weird to not have them, but then we
//                also need to track types outside the AST for efficient resolution of
//                types down the line (e.g. to support auto, polymorphism, etc).
void add_type_node(
    VALINOUT mflex::Tokens::const_iterator& curr_token,
    VALOUT mfast::AST& ast,
    VALOUT mfast::NodeBuffers& nodes,
    VALOUT mfast::ParserState& parser_state,
    VALOUT mfast::VariableTable& variable_table,
    VALINOUT mftype::TypeTable& type_table
) {
    // No validation needed for first token, guaranteed to be .
    assert(curr_token->type == mflex::TokenType::ASSIGN_TYPE);
    assert(maybe_see_type(curr_token + 1));

    nodes.strings.emplace_back(mfast::StringNode{
        curr_token, curr_token, std::move(curr_token->string) });

    auto vertex      = boost::add_vertex(ast);
    auto prev_vertex = parser_state.vertex[parser_state.cursor];
    parser_state.vertex[++parser_state.cursor] = vertex;
    nodes.vertex_node_map[vertex]              = &nodes.strings.back();

    boost::add_edge(prev_vertex, vertex, ast);

    ++parser_state.cursor;
    ++curr_token;
}

void mfast::parse(
    VALIN const mflex::Tokens& tokens,
    VALOUT AST&                ast,
    VALOUT NodeBuffers&        nodes,
    VALOUT VariableTable&      variable_table,
    VALOUT mftype::TypeTable& type_table
) {
    (void)tokens;

    // Ensure buffers are clear for building fresh AST.
    ast.clear();
    nodes.functions.clear();
    nodes.numbers.clear();
    nodes.operators.clear();
    nodes.variables.clear();
    nodes.nil_node = {};

    // TODO(Matthew): balance memory usage better.
    // Prepare stack for expectation, precedence, and associativity.
    ParserState parser_state;
    parser_state.cursor        = 0;
    parser_state.precedence    = new uint16_t[tokens.size()];
    parser_state.associativity = new Associativity[tokens.size()];
    parser_state.vertex        = new ASTVertex[tokens.size()];

    // Add top-level block node for module.
    parser_state.precedence[0]                    = parser_expects::EXPRESSION;
    parser_state.associativity[0]                 = Associativity::ANY;
    parser_state.vertex[0]                        = boost::add_vertex(ast);
    nodes.vertex_node_map[parser_state.vertex[0]] = &nodes.nil_node;

    // Iterate tokens, parsing as appropriate.
    auto it = tokens.begin();
    while (it != tokens.end()) {
        if (parser_state.precedence[parser_state.cursor] <= parser_expects::EXPRESSION)
        {
            switch (it->type) {
                case mflex::TokenType::IF:
                    // Add if vertex, push precedence parser_expects::ASSIGNMENT
                    add_if_node(...);
                    continue;
                case mflex::TokenType::FOR:
                    // Add for vertex, push precedence parser_expects::IDENTIFIER
                    add_for_node(...);
                    continue;
                case mflex::TokenType::WHILE:
                    // Add while vertex, push precedence parser_expects::EXPRESSION
                    add_while_node(...);
                    continue;
                case mflex::TokenType::PRINT:
                    // Add print vertex, push precedence parser_expects::EXPRESSION
                    add_print_node(...);
                    continue;
                case mflex::TokenType::LEFT_BRACE:
                    // Add block vertex, push precedence parser_expects::EXPRESSION
                    add_block_node(...);
                    continue;
                case mflex::TokenType::RIGHT_BRACE:
                    // Pop block.
                    parser_state.cursor -= 1;
                    continue;
            }
        }

        if (parser_state.precedence[parser_state.cursor] <= parser_expects::ASSIGNMENT)
        {
            // TODO(Matthew): sketch out implementation.
        }

        if (parser_state.precedence[parser_state.cursor] <= parser_expects::LOGIC) {
            switch (it->type) {
                case mflex::TokenType::AND:
                case mflex::TokenType::OR:
                    // Add if vertex, push precedence parser_expects::expression.
                    add_operator_node(...);
                    continue;
            }
        }

        if (parser_state.precedence[parser_state.cursor] <= parser_expects::EQUALITY) {
        }

        if (parser_state.precedence[parser_state.cursor] <= parser_expects::COMPARISON)
        {
            switch (it->type) {
                case mflex::TokenType::EQUALS:
                case mflex::TokenType::NOT_EQUALS:
                case mflex::TokenType::LESS_THAN:
                case mflex::TokenType::LESS_THAN_OR_EQUAL_TO:
                case mflex::TokenType::GREATER_THAN:
                case mflex::TokenType::GREATER_THAN_OR_EQUAL_TO:
                    // TODO(Matthew): grammar as written says we should push TERM and I
                    //                  think I want to disallow e.g. 4 < 3 > 4 <= 2
                    //                  so great
                    // Add operator vertex, push precedence parser_expects::TERM.
                    add_operator_node(...);
                    continue;
            }
        }

        if (parser_state.precedence[parser_state.cursor] <= parser_expects::TERM) {
            switch (it->type) {
                case mflex::TokenType::PLUS:
                case mflex::TokenType::MINUS:
                    // TODO(Matthew): grammar as written says we should push UNARY but I
                    //                  think I want to allow e.g. 4 + 3 + 4 - 2
                    // Add operator vertex, push precedence parser_expects::TERM.
                    add_operator_node(...);
                    continue;
            }
        }

        if (parser_state.precedence[parser_state.cursor] <= parser_expects::FACTOR) {
            switch (it->type) {
                case mflex::TokenType::SLASH:
                case mflex::TokenType::STAR:
                    // TODO(Matthew): grammar as written says we should push UNARY but I
                    //                  think I want to allow e.g. 4 * 3 * 4 / 2
                    // Add operator vertex, push precedence parser_expects::FACTOR.
                    add_operator_node(...);
                    continue;
            }
        }

        if (parser_state.precedence[parser_state.cursor] <= parser_expects::UNARY) {
            switch (it->type) {
                case mflex::TokenType::NOT:
                // TODO(Matthew): how do we differentiate subtraction and negation in
                //                  this parse scheme? can we just look back one token
                //                  or layer of stack, or should we do something else?
                case mflex::TokenType::MINUS:
                    // Add operator vertex, push precedence parser_expects::UNARY.
                    add_operator_node(...);
                    continue;
            }
        }

        if (parser_state.precedence[parser_state.cursor] <= parser_expects::CALL) {
            if (it->type == mflex::TokenType::IDENTIFIER
                && (it + 1)->type == mflex::TokenType::LEFT_PAREN)
            {
                // Add call vertex, push precendence parser_expects::EXPRESSION.
                add_call_node(...);
                continue;
            }
        }

        if (parser_state.precedence[parser_state.cursor] <= parser_expects::PRIMARY) {
            switch (it->type) {
                case mflex::TokenType::TRUE:
                case mflex::TokenType::FALSE:
                    // Add Boolean vertex, pop precedence.
                    add_bool_node(...);
                    continue;
                case mflex::TokenType::NIL:
                    // Add null vertex, pop precedence.
                    add_null_node(...);
                    continue;
                case mflex::TokenType::NUMBER:
                    // Add number vertex, pop precedence.
                    add_number_node(...);
                    continue;
                case mflex::TokenType::STRING:
                    // Add string vertex, pop precedence.
                    add_string_node(...);
                    continue;
                case mflex::TokenType::IDENTIFIER:
                    // TODO(Matthew): can identifiers be anything but variables?
                    //                  if so, add_identifier_node and resolve later.
                    // Add variable vertex, pop precedence.
                    add_variable_node(...);
                    continue;
                case mflex::TokenType::LEFT_PAREN:
                    // Add paren vertex, push precedence parser_expects::ASSIGNMENT.
                    add_paren_node(...);
                    continue;
                case mflex::TokenType::LEFT_BRACE:
                    // Add block vertex, push precedence parser_expects::EXPRESSION.
                    add_block_node(...);
                    continue;
                case mflex::TokenType::LEFT_BRACKET:
                    // TODO(Matthew): ambiguity as could be number range or list.
                    //                  need a way to decide which we are seeing here.
                    //                  are we happy with this approach?
                    if ((it + 2)->type == mflex::TokenType::RANGE) {
                        // Add range vertex, push precedence parser_expects::PRIMARY.
                        add_range_node(...);
                    } else {
                        // Add list vertex, push precedence parser_expects::EXPRESSION.
                        add_list_node(...);
                    }
                    continue;
            }
        }
    }

    // If we get here and cursor is not pointing to top-level block, then something has
    // gone wrong in parsing.
    assert(parser_state.cursor == 0);

    // Clean up.
    delete[] parser_state.precedence;
    delete[] parser_state.vertex;
    delete[] parser_state.associativity;
}
