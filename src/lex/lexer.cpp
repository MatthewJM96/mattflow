#include "stdafx.h"

#include "literal/identifier.h"
#include "literal/string.h"

#include "lex/lexer.h"

static void trim_whitespace(VALINOUT mf::SourceView& source_view) {
    auto offset = source_view.source.find_first_not_of(" \n");
    if (offset == std::string_view::npos) {
        source_view.source = std::string_view();

        source_view.start_line   = source_view.end_line;
        source_view.start_column = source_view.end_column;

        return;
    }

    for (auto c : source_view.source.substr(0, offset)) {
        if (c == '\n') {
            ++source_view.start_line;
            source_view.start_column = 0;
        } else {
            ++source_view.start_column;
        }
    }
    source_view.source = source_view.source.substr(offset);
}

void mflex::parse(SourceView source_view, VALOUT Tokens& tokens) {
    // Get handle on global identifier and string tables.
    mflit::IdentifierTable& identifier_table = mflit::IdentifierTable::get();
    mflit::StringTable&     string_table     = mflit::StringTable::get();

    // Set up tracker of remaining source.
    SourceView remaining_source_view = source_view;

    // Trim whitespace off from of source.
    trim_whitespace(remaining_source_view);

    // Make sure the token and string buffers are clear.
    tokens.clear();

    // Loop in which we match for first token in remaining source, then adjust view on
    // source to leave out that token and any following whitespace.
    while (remaining_source_view.source.size() > 0) {
        ////////////////////////////////////////////////////////////////////////////////
        // Single-line comments.

        if (remaining_source_view.source.starts_with("//")) {
            size_t next_new_line = remaining_source_view.source.find("\n");

            remaining_source_view.source
                = remaining_source_view.source.substr(next_new_line);
            remaining_source_view.start_line   += 1;
            remaining_source_view.start_column = 0;

            trim_whitespace(remaining_source_view);

            continue;
        }

        ////////////////////////////////////////////////////////////////////////////////
        // Multi-line comments.

        if (remaining_source_view.source.starts_with("/*")) {
            size_t end_of_multiline = remaining_source_view.source.find("*/");

            for (auto c : remaining_source_view.source.substr(0, end_of_multiline)) {
                if (c == '\n') {
                    remaining_source_view.start_line   += 1;
                    remaining_source_view.start_column = 0;
                } else {
                    remaining_source_view.start_column += 1;
                }
            }

            remaining_source_view.source
                = remaining_source_view.source.substr(end_of_multiline);

            trim_whitespace(remaining_source_view);

            continue;
        }

        ////////////////////////////////////////////////////////////////////////////////
        // Tokens

        Token token;
        token.type = TokenType::SENTINEL;

        if (try_match_token(
                remaining_source_view, token, identifier_table, string_table
            ))
        {
            tokens.emplace_back(token);
        }

        mfassert(token.type != TokenType::SENTINEL, "Unidentified Lexical Object");

        remaining_source_view.source
            = remaining_source_view.source.substr(token.length);
        remaining_source_view.start_line   = token.end_line;
        remaining_source_view.start_column = token.end_column;

        trim_whitespace(remaining_source_view);
    }
}
