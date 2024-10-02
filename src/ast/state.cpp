#include "stdafx.h"

#include "ast/link.h"
#include "ast/state.h"

void mfast::push_enclosure(
    VALIN mfast::NodeInfo&& enclosing_node_info,
    mfast::EnclosingProps   enclosing_category,
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
    if ((enclosing_category & EnclosingProps::ROOT) != EnclosingProps::ROOT) {
        parser_state.non_operating_vertices.back().emplace_back(enclosing_vertex);
        parser_state.last_seen.back() = NodeCategory::NONOP;
    }

    // Push new stacks for operating and non-operating vertices.
    parser_state.operating_vertices.push_back({});
    parser_state.non_operating_vertices.push_back({});

    // Push a new last-seen category entry.
    parser_state.last_seen.emplace_back(NodeCategory::NONE);

    // Push enclosure category.
    parser_state.enclosed_by.emplace_back(enclosing_category);
}

void mfast::pop_enclosure(
    [[maybe_unused]] EnclosingProps expected_enclosing_props,
    VALOUT AST&                     ast,
    VALOUT NodeBuffers&             nodes,
    VALOUT ParserState&             parser_state
) {
    mfassert(
        (expected_enclosing_props & EnclosingProps::ROOT) == EnclosingProps::ROOT
            || parser_state.enclosed_by.size() > 1,
        "Trying to pop non-root enclosures but ended up at root."
    );

    // Link any remaining operators in the enclosure.
    link_operations_on_stack(Precedence::NONE, ast, nodes, parser_state);

    mfassert(
        (parser_state.enclosed_by.back() & expected_enclosing_props)
            == expected_enclosing_props,
        "Trying to pop an enclosure that doesn't have expected properties."
    );

    mfassert(
        parser_state.operating_vertices.back().size() == 0,
        "Trying to pop an enclosure in which we couldn't link all operators."
    );

#if defined(DEBUG)
    if ((expected_enclosing_props & EnclosingProps::SINGLE_EXPR)
        == EnclosingProps::SINGLE_EXPR)
    {
        mfassert(
            parser_state.non_operating_vertices.back().size() == 1,
            "Trying to pop an enclosure expecting one expression, and go %ld "
            "expressions instead",
            parser_state.non_operating_vertices.back().size()
        );
    } else if ((expected_enclosing_props & EnclosingProps::MULTI_EXPR) == EnclosingProps::MULTI_EXPR)
    {
        mfassert(
            parser_state.non_operating_vertices.back().size() >= 1,
            "Trying to pop an enclosure expecting one or more expressions, got none."
        );
    }
#endif  // defined(DEBUG)

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

void mfast::pop_enclosures_up_to(
    [[maybe_unused]] EnclosingProps target_enclosing_props,
    VALOUT AST&                     ast,
    VALOUT NodeBuffers&             nodes,
    VALOUT ParserState&             parser_state,
    bool                            including /*= false*/
) {
    while (parser_state.enclosed_by.size() > 0) {
        bool found_target = (parser_state.enclosed_by.back() & target_enclosing_props)
                            == target_enclosing_props;

        // Only pop the current enclosure if it is either not the target or we are
        // including the target in the popping.
        if (!including && found_target) break;

        mfassert(
            (target_enclosing_props & EnclosingProps::ROOT) == EnclosingProps::ROOT
                || parser_state.enclosed_by.size() > 1,
            "Trying to pop non-root enclosures but ended up at root."
        );

        // Link any remaining operators in the enclosure.
        link_operations_on_stack(Precedence::NONE, ast, nodes, parser_state);

        mfassert(
            parser_state.operating_vertices.back().size() == 0,
            "Trying to pop an enclosure in which we couldn't link all operators."
        );

#if defined(DEBUG)
        if ((target_enclosing_props & EnclosingProps::SINGLE_EXPR)
            == EnclosingProps::SINGLE_EXPR)
        {
            mfassert(
                parser_state.non_operating_vertices.back().size() == 1,
                "Trying to pop an enclosure expecting one expression, and go %ld "
                "expressions instead",
                parser_state.non_operating_vertices.back().size()
            );
        } else if ((target_enclosing_props & EnclosingProps::MULTI_EXPR) == EnclosingProps::MULTI_EXPR)
        {
            mfassert(
                parser_state.non_operating_vertices.back().size() >= 1,
                "Trying to pop an enclosure expecting one or more expressions, got "
                "none."
            );
        }
#endif  // defined(DEBUG)

        // TODO(Matthew): update the token iterator for end of enclosure.

        // Get remaining vertices of the enclosure and link to enclosing vertex.
        for (auto remaining_vertex : parser_state.non_operating_vertices.back())
            boost::add_edge(
                parser_state.enclosing_vertices.back(), remaining_vertex, ast
            );

        // Pop stacks and values from parser state.
        parser_state.precedence.pop_back();
        parser_state.associativity.pop_back();
        parser_state.operating_vertices.pop_back();
        parser_state.non_operating_vertices.pop_back();
        parser_state.enclosing_vertices.pop_back();
        parser_state.enclosed_by.pop_back();
        parser_state.last_seen.pop_back();

        if (found_target) break;
    }
}

void mfast::pop_enclosures_up_to_and_including(
    [[maybe_unused]] EnclosingProps target_enclosing_props,
    VALOUT AST&                     ast,
    VALOUT NodeBuffers&             nodes,
    VALOUT ParserState&             parser_state
) {
    pop_enclosures_up_to(target_enclosing_props, ast, nodes, parser_state, true);
}
