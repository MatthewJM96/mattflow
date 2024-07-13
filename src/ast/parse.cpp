#include "stdafx.h"

#include "ast/node.h"

#include "ast/parse.h"

void mfast::parse(
    VALIN const mflex::Tokens& tokens, VALOUT AST& ast, VALOUT NodeBuffers& nodes
) {
    (void)tokens;

    // Ensure buffers are clear for building fresh AST.
    ast.clear();
    nodes.functions.clear();
    nodes.numbers.clear();
    nodes.operators.clear();
    nodes.variables.clear();
    nodes.nil_node = {};

    // Add top-level expression node for module.
    auto top_level_vert                   = boost::add_vertex(ast);
    nodes.vertex_node_map[top_level_vert] = &nodes.nil_node;

    // Iterate tokens, parsing as appropriate.
    for (auto it = tokens.begin(); it != tokens.end(); ++it) {
    }
}
