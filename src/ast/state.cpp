#include "stdafx.h"

#include "ast/link.h"
#include "ast/state.h"

void mfast::push_enclosure(
    VALIN mfast::NodeInfo&&  enclosing_node_info,
    mfast::EnclosingCategory enclosing_category,
    VALOUT mfast::AST& ast,
    VALOUT mfast::NodeBuffers& nodes,
    VALOUT mfast::ParserState& parser_state
) {
    // Push new precedence and associativity tracking.
    parser_state.precedence.emplace_back(Precedence::NONE);
    parser_state.associativity.emplace_back(Associativity::SENTINEL);

    // Push a new vertex representing the enclosure.
    auto enclosing_vertex = boost::add_vertex(ast);
    parser_state.enclosing_vertices.emplace_back(enclosing_vertex);

    // Add node info for enclosure.
    nodes.vertex_node_map[enclosing_vertex] = nodes.node_info.size();
    nodes.node_info.emplace_back(std::forward<mfast::NodeInfo>(enclosing_node_info));

    // Push enclosing vertex onto the stack below.
    if (enclosing_category != EnclosingCategory::ROOT)
        parser_state.non_operating_vertices.back().emplace_back(enclosing_vertex);

    // Push new stacks for operating and non-operating vertices.
    parser_state.operating_vertices.push_back({});
    parser_state.non_operating_vertices.push_back({});

    // Push a new last-seen category entry.
    parser_state.last_seen.emplace_back(NodeCategory::NONE);

    // Push enclosure category.
    parser_state.enclosed_by.emplace_back(enclosing_category);
}

void mfast::pop_enclosure(
    mfast::EnclosingCategory expected_enclosing_category,
    VALOUT mfast::AST& ast,
    VALOUT mfast::NodeBuffers& nodes,
    VALOUT mfast::ParserState& parser_state
) {
    // Link any remaining operators in the enclosure.
    link_operations_on_stack(Precedence::NONE, ast, nodes, parser_state);

    mfassert(
        (parser_state.enclosed_by.back() & expected_enclosing_category)
            == parser_state.enclosed_by.back(),
        "Latest closure is different to expected during pop."
    );

    mfassert(
        parser_state.operating_vertices.back().size() == 0
            && parser_state.non_operating_vertices.back().size() >= 1,
        "Trying to pop an enclosure in which there is either a lack or excess of "
        "vertices on stack."
    );

    // TODO(Matthew): update the token iterator for end of enclosure.

    // Get remaining vertices of the enclosure and link to enclosing vertex.
    for (auto remaining_vertex : parser_state.non_operating_vertices.back())
        boost::add_edge(parser_state.enclosing_vertices.back(), remaining_vertex, ast);

    // Pop stacks and values from parser state.
    parser_state.precedence.pop_back();
    parser_state.associativity.pop_back();
    parser_state.operating_vertices.pop_back();
    parser_state.non_operating_vertices.pop_back();
    parser_state.enclosing_vertices.pop_back();
    parser_state.enclosed_by.pop_back();
    parser_state.last_seen.pop_back();
}
