#include "stdafx.h"

#include "ast/node.h"
#include "literal/identifier.h"

#include "ast/parse.h"

// bool maybe_see_type(VALIN const mflex::Tokens::const_iterator& token) {
//     return token->type == mflex::TokenType::IDENTIFIER
//            || token->type == mflex::TokenType::STRUCT
//            || token->type == mflex::TokenType::LEFT_PAREN;
// }

// void add_if_node(
//     VALINOUT mflex::Tokens::const_iterator& curr_token,
//     VALOUT mfast::AST& ast,
//     VALOUT mfast::NodeBuffers& nodes,
//     VALOUT mfast::ParserState& parser_state
// ) {
//     // Should only get here after knowing this is true.
//     assert(curr_token->type == mflex::TokenType::IF);

//     // No validation needed for if node as only requirement is that we have seen an
//     "if"
//     // token - which to get here we certainly have.

//     // TODO(Matthew): do we want to relax this as we might want to allow e.g.
//     //                  if x == if y then z else w then
//     //                but it is ugly.
//     if (parser_state.precedence[parser_state.cursor]
//         > mfast::parser_expects::EXPRESSION)
//     {
//         exit(1);
//     }

//     // Increment parser cursor.
//     //   Layer below is: expression-accepting node.
//     parser_state.cursor += 1;

//     // Add vertex to AST for if node, and push it onto the stack.
//     auto vertex                              = boost::add_vertex(ast);
//     auto prev_vertex                         =
//     parser_state.vertex[parser_state.cursor];
//     parser_state.vertex[parser_state.cursor] = vertex;

//     // Set parser expects.
//     parser_state.precedence[parser_state.cursor] = mfast::parser_expects::ASSIGNMENT;

//     // Create the if node.
//     nodes.ifs.emplace_back(curr_token, curr_token);
//     nodes.vertex_node_map[vertex] = &nodes.ifs.back();

//     // Link if node to previous.
//     boost::add_edge(prev_vertex, vertex, ast);

//     // Move forward a token.
//     curr_token += 1;
// }

// void add_for_node(
//     VALINOUT mflex::Tokens::const_iterator& curr_token,
//     VALOUT mfast::AST& ast,
//     VALOUT mfast::NodeBuffers& nodes,
//     VALOUT mfast::ParserState& parser_state
// ) {
//     // Should only get here after knowing this is true.
//     assert(curr_token->type == mflex::TokenType::FOR);

//     // No validation needed for for node as only requirement is that we have seen an
//     // "for" token - which to get here we certainly have.

//     // TODO(Matthew): do we want to relax this as we might want to allow e.g.
//     //                  if x == for y in [ 1 .. 5 ] do y * y
//     //                but it is ugly.
//     if (parser_state.precedence[parser_state.cursor]
//         > mfast::parser_expects::EXPRESSION)
//     {
//         exit(1);
//     }

//     // Increment parser cursor.
//     //   Layer below is: expression-accepting node.
//     parser_state.cursor += 1;

//     // Add vertex to AST for for node, and push it onto the stack.
//     auto vertex                              = boost::add_vertex(ast);
//     auto prev_vertex                         =
//     parser_state.vertex[parser_state.cursor];
//     parser_state.vertex[parser_state.cursor] = vertex;

//     // Set parser expects.
//     parser_state.precedence[parser_state.cursor] = mfast::parser_expects::IDENTIFIER;

//     // Create the for node.
//     nodes.fors.emplace_back(curr_token, curr_token);
//     nodes.vertex_node_map[vertex] = &nodes.fors.back();

//     // Link for node to previous.
//     boost::add_edge(prev_vertex, vertex, ast);

//     // Move forward a token.
//     curr_token += 1;
// }

// void add_range_node(
//     VALINOUT mflex::Tokens::const_iterator& curr_token,
//     VALOUT mfast::AST& ast,
//     VALOUT mfast::NodeBuffers& nodes,
//     VALOUT mfast::ParserState& parser_state
// ) {
//     // Should only get here after knowing this is true.
//     assert(curr_token->type == mflex::TokenType::IDENTIFIER);
//     assert((curr_token + 1)->type == mflex::TokenType::IN);

//     // No validation needed for for node as only requirement is that we have seen an
//     // "for" token - which to get here we certainly have.

//     if (parser_state.precedence[parser_state.cursor]
//         > mfast::parser_expects::IDENTIFIER)
//     {
//         exit(1);
//     }

//     // Increment parser cursor.
//     //   Layer below is: for node
//     parser_state.cursor += 1;

//     // Add vertex to AST for range node, and push it onto the stack.
//     auto vertex                              = boost::add_vertex(ast);
//     auto prev_vertex                         =
//     parser_state.vertex[parser_state.cursor];
//     parser_state.vertex[parser_state.cursor] = vertex;

//     // Set parser expects.
//     parser_state.precedence[parser_state.cursor] = mfast::parser_expects::EXPRESSION;

//     // Create the range node.
//     nodes.ranges.emplace_back(curr_token, curr_token);
//     nodes.vertex_node_map[vertex] = &nodes.fors.back();

//     // Link range node to previous.
//     boost::add_edge(prev_vertex, vertex, ast);

//     // Increment token, then add identifier (incrementing a second time).
//     curr_token += 1;
//     try_add_identifier_node(...);
// }

// void add_number_node(
//     VALINOUT mflex::Tokens::const_iterator& curr_token,
//     VALOUT mfast::AST& ast,
//     VALOUT mfast::NodeBuffers& nodes,
//     VALOUT mfast::ParserState& parser_state
// ) {
//     // No validation needed here, the current token will be a number token which is
//     // assured to be valid during lexing.
//     assert(curr_token->type == mflex::TokenType::NUMBER);

//     nodes.numbers.emplace_back(mfast::NumberNode{
//         curr_token, curr_token, curr_token->number });

//     auto vertex      = boost::add_vertex(ast);
//     auto prev_vertex = parser_state.vertex[parser_state.cursor];
//     parser_state.vertex[++parser_state.cursor] = vertex;
//     nodes.vertex_node_map[vertex]              = &nodes.numbers.back();

//     boost::add_edge(prev_vertex, vertex, ast);

//     // number
//     curr_token += 1;
// }

// void add_string_node(
//     VALINOUT mflex::Tokens::const_iterator& curr_token,
//     VALOUT mfast::AST& ast,
//     VALOUT mfast::NodeBuffers& nodes,
//     VALOUT mfast::ParserState& parser_state
// ) {
//     // No validation needed here, the current token will be a string token which is
//     // assured to be valid during lexing.
//     assert(curr_token->type == mflex::TokenType::STRING);

//     nodes.strings.emplace_back(mfast::StringNode{
//         curr_token, curr_token, std::move(curr_token->string) });

//     auto vertex      = boost::add_vertex(ast);
//     auto prev_vertex = parser_state.vertex[parser_state.cursor];
//     parser_state.vertex[++parser_state.cursor] = vertex;
//     nodes.vertex_node_map[vertex]              = &nodes.strings.back();

//     boost::add_edge(prev_vertex, vertex, ast);

//     // string
//     curr_token += 1;
// }

// void add_variable_declaration_node(
//     VALINOUT mflex::Tokens::const_iterator& curr_token,
//     VALOUT mfast::AST& ast,
//     VALOUT mfast::NodeBuffers& nodes,
//     VALOUT mfast::ParserState& parser_state,
//     VALINOUT mftype::TypeTable& type_table
// ) {
//     // No validation needed here, the current token will be an identifier token which
//     is
//     // assured to be valid during lexing, and the type assignment character ":".
//     assert(curr_token->type == mflex::TokenType::IDENTIFIER);
//     assert((curr_token + 1)->type == mflex::TokenType::ASSIGN_TYPE);

//     // TODO(Matthew): this will need revisiting to deal with scope.
//     if (type_table.contains(curr_token->identifier_idx)) {
//         auto& ident = mflit::IdentifierTable::get();
//         std::cout << "Variable " << ident.get(curr_token->identifier_idx)
//                   << " already declared." << std::endl;
//         exit(456);
//     }

//     // Mark variable as existing, next token should assign type and if it doesn't we
//     // have an issue.
//     type_table[curr_token->identifier_idx] = nullptr;

//     nodes.variables.emplace_back(mfast::VariableNode{
//         curr_token, curr_token, curr_token->identifier_idx });

//     auto vertex      = boost::add_vertex(ast);
//     auto prev_vertex = parser_state.vertex[parser_state.cursor];
//     parser_state.vertex[++parser_state.cursor] = vertex;
//     nodes.vertex_node_map[vertex]              = &nodes.variables.back();

//     boost::add_edge(prev_vertex, vertex, ast);

//     // identifier ":"
//     curr_token += 2;
// }

// void add_assignment_node(
//     VALINOUT mflex::Tokens::const_iterator& curr_token,
//     VALOUT mfast::AST& ast,
//     VALOUT mfast::NodeBuffers& nodes,
//     VALOUT mfast::ParserState& parser_state,
//     VALINOUT mftype::TypeTable& type_table
// ) {
//     // No validation needed here, the current token will be the value assignment
//     // character "=".
//     assert(curr_token->type == mflex::TokenType::ASSIGN_VALUE);

//     nodes.assignments.emplace_back(mfast::AssignmentNode{ curr_token, curr_token });

//     auto vertex      = boost::add_vertex(ast);
//     auto prev_vertex = parser_state.vertex[parser_state.cursor];
//     parser_state.vertex[++parser_state.cursor] = vertex;
//     nodes.vertex_node_map[vertex]              = &nodes.assignments.back();

//     boost::add_edge(prev_vertex, vertex, ast);

//     // "="
//     curr_token += 1;
// }

// void add_struct_node(
//     VALINOUT mflex::Tokens::const_iterator& curr_token,
//     VALOUT mfast::AST& ast,
//     VALOUT mfast::NodeBuffers& nodes,
//     VALOUT mfast::ParserState& parser_state,
//     VALINOUT mftype::TypeTable& type_table
// ) {
//     // No validation needed here, the current token will be a string token which is
//     // assured to be valid during lexing.
//     assert(curr_token->type == mflex::TokenType::STRUCT);
//     assert((curr_token + 1)->type == mflex::TokenType::LEFT_BRACE);

//     nodes.structs.emplace_back(mfast::StructNode{
//         curr_token, curr_token, type_table.register_type_from_token(*curr_token) });

//     auto vertex      = boost::add_vertex(ast);
//     auto prev_vertex = parser_state.vertex[parser_state.cursor];
//     parser_state.vertex[++parser_state.cursor] = vertex;
//     nodes.vertex_node_map[vertex]              = &nodes.structs.back();

//     boost::add_edge(prev_vertex, vertex, ast);

//     // "struct" "{"
//     curr_token += 2;
// }

// void add_struct_field_node(
//     VALINOUT mflex::Tokens::const_iterator& curr_token,
//     VALOUT mfast::AST& ast,
//     VALOUT mfast::NodeBuffers& nodes,
//     VALOUT mfast::ParserState& parser_state,
//     VALINOUT mftype::TypeTable& type_table
// ) {
//     // TODO(Matthew): implement correctly... just copied add_struct_node.

//     // No validation needed here, the current token will be a string token which is
//     // assured to be valid during lexing.
//     assert(curr_token->type == mflex::TokenType::IDENTIFIER);
//     assert((curr_token + 1)->type == mflex::TokenType::LEFT_BRACE);

//     nodes.struct_fields.emplace_back(mfast::StructFieldNode{
//         curr_token, curr_token, type_table.register_type_from_token(*curr_token) });

//     auto vertex      = boost::add_vertex(ast);
//     auto prev_vertex = parser_state.vertex[parser_state.cursor];
//     parser_state.vertex[++parser_state.cursor] = vertex;
//     nodes.vertex_node_map[vertex]              = &nodes.strings.back();

//     boost::add_edge(prev_vertex, vertex, ast);

//     // "struct" "{"
//     curr_token += 2;
// }

// // TODO(Matthew): do we want type nodes? it seems weird to not have them, but then we
// //                also need to track types outside the AST for efficient resolution
// of
// //                types down the line (e.g. to support auto, polymorphism, etc).
// void add_type_node(
//     VALINOUT mflex::Tokens::const_iterator& curr_token,
//     VALOUT mfast::AST& ast,
//     VALOUT mfast::NodeBuffers& nodes,
//     VALOUT mfast::ParserState& parser_state,
//     VALINOUT mftype::TypeTable& type_table
// ) {
//     // No validation needed for first token, guaranteed to be .
//     assert(curr_token->type == mflex::TokenType::ASSIGN_TYPE);
//     assert(maybe_see_type(curr_token + 1));

//     nodes.strings.emplace_back(mfast::StringNode{
//         curr_token, curr_token, std::move(curr_token->string) });

//     auto vertex      = boost::add_vertex(ast);
//     auto prev_vertex = parser_state.vertex[parser_state.cursor];
//     parser_state.vertex[++parser_state.cursor] = vertex;
//     nodes.vertex_node_map[vertex]              = &nodes.strings.back();

//     boost::add_edge(prev_vertex, vertex, ast);

//     ++parser_state.cursor;
//     ++curr_token;
// }

void add_bool_node(
    VALINOUT mflex::Tokens::const_iterator& curr_token,
    VALOUT mfast::AST& ast,
    VALOUT mfast::NodeBuffers& nodes,
    VALOUT mfast::ParserState& parser_state
) {
    // Should only get here after knowing this is true.
    assert(
        curr_token->type == mflex::TokenType::FALSE
        || curr_token->type == mflex::TokenType::TRUE
    );

    // No validation needed for bool node as only requirement is that we have seen a
    // boolean token - which to get here we certainly have.

    // Add vertex to AST for bool node, and push it onto the stack.
    auto vertex = boost::add_vertex(ast);
    parser_state.vertices.emplace_back(vertex);

    // Add node info about bool node and associate with vertex in AST.
    nodes.vertex_node_map[vertex] = &nodes.node_info.emplace_back(mfast::BoolNode{
        curr_token, curr_token, curr_token->type == mflex::TokenType::TRUE });

    // Move forward a token.
    curr_token += 1;
}

void add_null_node(
    VALINOUT mflex::Tokens::const_iterator& curr_token,
    VALOUT mfast::AST& ast,
    VALOUT mfast::NodeBuffers& nodes,
    VALOUT mfast::ParserState& parser_state
) {
    // Should only get here after knowing this is true.
    assert(curr_token->type == mflex::TokenType::NIL);

    // No validation needed for bool node as only requirement is that we have seen a
    // boolean token - which to get here we certainly have.

    // Add vertex to AST for bool node, and push it onto the stack.
    auto vertex = boost::add_vertex(ast);
    parser_state.vertices.emplace_back(vertex);

    // Add node info about bool node and associate with vertex in AST.
    nodes.vertex_node_map[vertex]
        = &nodes.node_info.emplace_back(mfast::NullNode{ curr_token, curr_token });

    // Move forward a token.
    curr_token += 1;
}

void add_number_node(
    VALINOUT mflex::Tokens::const_iterator& curr_token,
    VALOUT mfast::AST& ast,
    VALOUT mfast::NodeBuffers& nodes,
    VALOUT mfast::ParserState& parser_state
) {
    // Should only get here after knowing this is true.
    assert(curr_token->type == mflex::TokenType::NUMBER);

    // No validation needed for bool node as only requirement is that we have seen a
    // boolean token - which to get here we certainly have.

    // Add vertex to AST for bool node, and push it onto the stack.
    auto vertex = boost::add_vertex(ast);
    parser_state.vertices.emplace_back(vertex);

    // Add node info about bool node and associate with vertex in AST.
    nodes.vertex_node_map[vertex] = &nodes.node_info.emplace_back(mfast::NumberNode{
        curr_token, curr_token, curr_token->number });

    // Move forward a token.
    curr_token += 1;
}

void add_string_node(
    VALINOUT mflex::Tokens::const_iterator& curr_token,
    VALOUT mfast::AST& ast,
    VALOUT mfast::NodeBuffers& nodes,
    VALOUT mfast::ParserState& parser_state
) {
    // Should only get here after knowing this is true.
    assert(curr_token->type == mflex::TokenType::STRING);

    // No validation needed for bool node as only requirement is that we have seen a
    // boolean token - which to get here we certainly have.

    // Add vertex to AST for bool node, and push it onto the stack.
    auto vertex = boost::add_vertex(ast);
    parser_state.vertices.emplace_back(vertex);

    // Add node info about bool node and associate with vertex in AST.
    nodes.vertex_node_map[vertex] = &nodes.node_info.emplace_back(mfast::StringNode{
        curr_token, curr_token, std::move(curr_token->string) });

    // Move forward a token.
    curr_token += 1;
}

void mfast::parse(
    VALIN const mflex::Tokens& tokens,
    VALOUT AST&                ast,
    VALOUT NodeBuffers&        nodes,
    VALOUT mftype::TypeTable& type_table
) {
    (void)type_table;

    // Ensure buffers are clear for building fresh AST.
    ast.clear();
    nodes.node_info.clear();
    nodes.nil_node        = {};
    nodes.vertex_node_map = boost::get(vertex_data, ast);

    // Prepare stack for precedence, associativity, and last attachable vertex with the
    // top-level block node for module.
    ParserState parser_state;
    parser_state.precedence.emplace_back(Precedence::NONE);
    parser_state.associativity.emplace_back(Associativity::NONE);
    parser_state.vertices.emplace_back(boost::add_vertex(ast));
    nodes.vertex_node_map[parser_state.vertices.back()] = &nodes.nil_node;

    // Iterate tokens, parsing as appropriate.
    auto it = tokens.begin();
    while (it != tokens.end()) {
        switch (it->type) {
            // case mflex::TokenType::IF:
            //     // Add if vertex, push precedence parser_expects::ASSIGNMENT
            //     add_if_node(...);
            //     continue;
            // case mflex::TokenType::THEN:
            //     // Add if vertex, push precedence parser_expects::EXPRESSION
            //     add_then_node(...);
            //     continue;
            // case mflex::TokenType::ELIF:
            //     // Add if vertex, push precedence parser_expects::ASSIGNMENT
            //     add_elif_node(...);
            //     continue;
            // case mflex::TokenType::ELSE:
            //     // Add if vertex, push precedence parser_expects::EXPRESSION
            //     add_else_node(...);
            //     continue;
            // case mflex::TokenType::FOR:
            //     // Add for vertex, push precedence parser_expects::IDENTIFIER
            //     add_for_node(...);
            //     continue;
            // case mflex::TokenType::IN:
            //     // Add in vertex, push precedence parser_expects::EXPRESSION
            //     add_in_node(...);
            //     continue;
            // case mflex::TokenType::WHERE:
            //     // Add where vertex, push precedence parser_expects::LOGIC
            //     add_where_node(...);
            //     continue;
            // case mflex::TokenType::WHILE:
            //     // Add while vertex, push precedence parser_expects::EXPRESSION
            //     add_while_node(...);
            //     continue;
            // case mflex::TokenType::DO:
            //     // Add do vertex, push precedence parser_expects::EXPRESSION
            //     add_do_node(...);
            //     continue;
            // case mflex::TokenType::PRINT:
            //     // Add print vertex, push precedence parser_expects::EXPRESSION
            //     add_print_node(...);
            //     continue;
            // case mflex::TokenType::LEFT_BRACE:
            //     // Add block vertex, push precedence parser_expects::EXPRESSION
            //     add_block_node(...);
            //     continue;
            // case mflex::TokenType::RIGHT_BRACE:
            //     // Pop block.
            //     pop_block(...);
            //     continue;
            // TODO(Matthew): sketch out assignment implementation.
            // case mflex::TokenType::AND:
            // case mflex::TokenType::OR:
            //     // Add if vertex, push precedence parser_expects::expression.
            //     add_logic_node(...);
            //     continue;
            // case mflex::TokenType::EQUALS:
            // case mflex::TokenType::NOT_EQUALS:
            //     // TODO(Matthew): grammar as written says we should push COMPARISON
            //     and
            //     // I
            //     //                  and I think I want to disallow e.g.
            //     //                      4 != 3 != 4 == 2
            //     //                  so great
            //     // Add operator vertex, push precedence parser_expects::COMPARISON.
            //     add_equality_node(...);
            //     continue;
            // case mflex::TokenType::LESS_THAN:
            // case mflex::TokenType::LESS_THAN_OR_EQUAL_TO:
            // case mflex::TokenType::GREATER_THAN:
            // case mflex::TokenType::GREATER_THAN_OR_EQUAL_TO:
            //     // TODO(Matthew): grammar as written says we should push TERM and I
            //     //                  think I want to disallow e.g. 4 < 3 > 4 <= 2
            //     //                  so great
            //     // Add operator vertex, push precedence parser_expects::TERM.
            //     add_comparison_node(...);
            //     continue;
            // case mflex::TokenType::PLUS:
            // case mflex::TokenType::MINUS:
            //     // TODO(Matthew): grammar as written says we should push UNARY but I
            //     //                  think I want to allow e.g. 4 + 3 + 4 - 2
            //     // Add operator vertex, push precedence parser_expects::TERM.
            //     add_term_node(...);
            //     continue;
            // case mflex::TokenType::SLASH:
            // case mflex::TokenType::STAR:
            //     // TODO(Matthew): grammar as written says we should push UNARY but I
            //     //                  think I want to allow e.g. 4 * 3 * 4 / 2
            //     // Add operator vertex, push precedence parser_expects::FACTOR.
            //     add_factor_node(...);
            //     continue;
            // case mflex::TokenType::NOT:
            // // TODO(Matthew): how do we differentiate subtraction and negation in
            // //                  this parse scheme? can we just look back one token
            // //                  or layer of stack, or should we do something else?
            // case mflex::TokenType::MINUS:
            //     // Add operator vertex, push precedence parser_expects::UNARY.
            //     add_unary_node(...);
            //     continue;
            // case mflex::TokenType::IDENTIFIER:
            //     if ((it + 1)->type == mflex::TokenType::LEFT_PAREN) {
            //         // Add call vertex, push precendence parser_expects::EXPRESSION.
            //         add_call_node(...);
            //         continue;
            //     } else if ((it + 1)->type == mflex::TokenType::IN) {
            //         // Add range vertex, and associate identifier, push precedence
            //         // parser_expects::EXPRESSION
            //         add_range_node(...);
            //     } else {
            //         // TODO(Matthew): can identifiers be anything but variables?
            //         //                  if so, add_identifier_node and resolve
            //         //                  later.
            //         // Add variable vertex, pop precedence.
            //         add_variable_node(...);
            //         continue;
            //     }
            case mflex::TokenType::IF:
            case mflex::TokenType::THEN:
            case mflex::TokenType::ELIF:
            case mflex::TokenType::ELSE:
            case mflex::TokenType::FOR:
            case mflex::TokenType::IN:
            case mflex::TokenType::WHERE:
            case mflex::TokenType::WHILE:
            case mflex::TokenType::DO:
            case mflex::TokenType::PRINT:
            case mflex::TokenType::LEFT_BRACE:
            case mflex::TokenType::RIGHT_BRACE:
            case mflex::TokenType::AND:
            case mflex::TokenType::OR:
            case mflex::TokenType::EQUALS:
            case mflex::TokenType::NOT_EQUALS:
            case mflex::TokenType::LESS_THAN:
            case mflex::TokenType::LESS_THAN_OR_EQUAL_TO:
            case mflex::TokenType::GREATER_THAN:
            case mflex::TokenType::GREATER_THAN_OR_EQUAL_TO:
            case mflex::TokenType::PLUS:
            case mflex::TokenType::MINUS:
            case mflex::TokenType::SLASH:
            case mflex::TokenType::STAR:
            case mflex::TokenType::NOT:
            case mflex::TokenType::IDENTIFIER:
            case mflex::TokenType::LEFT_PAREN:
            case mflex::TokenType::RIGHT_PAREN:
            case mflex::TokenType::LEFT_BRACKET:
            case mflex::TokenType::RIGHT_BRACKET:
            case mflex::TokenType::COMMA:
            case mflex::TokenType::RANGE:
            case mflex::TokenType::DOT:
            case mflex::TokenType::ARROW:
            case mflex::TokenType::ASSIGN_TYPE:
            case mflex::TokenType::ASSIGN_VALUE:
            case mflex::TokenType::MATCH:
            case mflex::TokenType::STRUCT:
            case mflex::TokenType::CHAR:
            case mflex::TokenType::BOOL:
            case mflex::TokenType::INT:
            case mflex::TokenType::INT8:
            case mflex::TokenType::INT16:
            case mflex::TokenType::INT32:
            case mflex::TokenType::INT64:
            case mflex::TokenType::UINT:
            case mflex::TokenType::UINT8:
            case mflex::TokenType::UINT16:
            case mflex::TokenType::UINT32:
            case mflex::TokenType::UINT64:
            case mflex::TokenType::FLOAT32:
            case mflex::TokenType::FLOAT64:
            case mflex::TokenType::SENTINEL:
                break;
            case mflex::TokenType::TRUE:
            case mflex::TokenType::FALSE:
                // Add Boolean vertex, pop precedence.
                add_bool_node(it, ast, nodes, parser_state);
                continue;
            case mflex::TokenType::NIL:
                // Add null vertex, pop precedence.
                add_null_node(it, ast, nodes, parser_state);
                continue;
            case mflex::TokenType::NUMBER:
                // Add number vertex, pop precedence.
                add_number_node(it, ast, nodes, parser_state);
                continue;
            case mflex::TokenType::STRING:
                // Add string vertex, pop precedence.
                add_string_node(it, ast, nodes, parser_state);
                continue;
                // case mflex::TokenType::LEFT_PAREN:
                //     // Add paren vertex, push precedence parser_expects::ASSIGNMENT.
                //     add_paren_node(it, ast, nodes, parser_state);
                //     continue;
                // case mflex::TokenType::LEFT_BRACKET:
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
        }
    }

    // If we get here and cursor is not pointing to top-level block, then something has
    // gone wrong in parsing.
    // assert(parser_state.vertices.size() == 0);
}
