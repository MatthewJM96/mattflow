#include "stdafx.h"

#include "ast/node.h"
#include "literal/identifier.h"

#include "ast/parse.h"

bool maybe_see_type(VALIN const mflex::Tokens::const_iterator& token) {
    return token->type == mflex::TokenType::IDENTIFIER
           || token->type == mflex::TokenType::STRUCT
           || token->type == mflex::TokenType::LEFT_PAREN;
}

mfast::ASTVertex add_number_node(
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

    mfast::ASTVertex vertex                  = boost::add_vertex(ast);
    parser_state.vertex[parser_state.cursor] = vertex;
    nodes.vertex_node_map[vertex]            = &nodes.numbers.back();

    ++parser_state.cursor;
    ++curr_token;

    return vertex;
}

mfast::ASTVertex add_string_node(
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

    mfast::ASTVertex vertex                  = boost::add_vertex(ast);
    parser_state.vertex[parser_state.cursor] = vertex;
    nodes.vertex_node_map[vertex]            = &nodes.strings.back();

    ++parser_state.cursor;
    ++curr_token;

    return vertex;
}

mfast::ASTVertex add_variable_declaration_node(
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

    mfast::ASTVertex vertex                  = boost::add_vertex(ast);
    parser_state.vertex[parser_state.cursor] = vertex;
    nodes.vertex_node_map[vertex]            = &nodes.variables.back();

    ++parser_state.cursor;

    return vertex;
}

mfast::ASTVertex add_struct_node(
    VALINOUT mflex::Tokens::const_iterator& curr_token,
    VALOUT mfast::AST& ast,
    VALOUT mfast::NodeBuffers& nodes,
    VALOUT mfast::ParserState& parser_state,
    VALOUT mfast::VariableTable& variable_table,
    VALINOUT mftype::TypeTable& type_table
) { }

mfast::ASTVertex add_struct_field_node(
    VALINOUT mflex::Tokens::const_iterator& curr_token,
    VALOUT mfast::AST& ast,
    VALOUT mfast::NodeBuffers& nodes,
    VALOUT mfast::ParserState& parser_state,
    VALOUT mfast::VariableTable& variable_table,
    VALINOUT mftype::TypeTable& type_table
) { }

// TODO(Matthew): do we want type nodes? it seems weird to not have them, but then we
//                also need to track types outside the AST for efficient resolution of
//                types down the line (e.g. to support auto, polymorphism, etc).
mfast::ASTVertex add_type_node(
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

    mfast::ASTVertex vertex                  = boost::add_vertex(ast);
    parser_state.vertex[parser_state.cursor] = vertex;
    nodes.vertex_node_map[vertex]            = &nodes.strings.back();

    ++parser_state.cursor;
    ++curr_token;

    return vertex;
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
    parser_state.expects       = new int[tokens.size()];
    parser_state.precedence    = new int[tokens.size()];
    parser_state.associativity = new Associativity[tokens.size()];
    parser_state.vertex        = new ASTVertex[tokens.size()];

    // Add top-level block node for module.
    parser_state.expects[0]                       = parser_expects::ALL;
    parser_state.precedence[0]                    = -1;
    parser_state.associativity[0]                 = Associativity::ANY;
    parser_state.vertex[0]                        = boost::add_vertex(ast);
    nodes.vertex_node_map[parser_state.vertex[0]] = &nodes.nil_node;

    // Iterate tokens, parsing as appropriate.
    for (auto it = tokens.begin(); it != tokens.end(); ++it) {
    }
}
