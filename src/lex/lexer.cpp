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
        Token token;
        token.type = TokenType::SENTINEL;

        // For each regex pattern, try to match to the remaining source.
        for (auto [token_type, token_pattern] : TOKEN_REGEX_PATTERNS) {
            std::cmatch results;

            // Try to match current regex pattern.
            //   NOTE: reinterpret_cast needed to satisfy MSVC.
            bool found_match = std::regex_search(
#if defined(MATTFLOW_COMPILER_MSVC)
                *remaining_source_view.source.begin(),
                *remaining_source_view.source.end(),
#else   // defined(MATTFLOW_COMPILER_MSVC)
                remaining_source_view.source.begin(),
                remaining_source_view.source.end(),
#endif  // defined(MATTFLOW_COMPILER_MSVC)
                results,
                std::regex("^(?:" + token_pattern + ")")
            );

            // Construct token if we matched.
            if (found_match) {
                token.type    = token_type;
                token.file_id = source_view.file_id;

                token.length       = results[0].length();
                token.start_line   = remaining_source_view.start_line;
                token.start_column = remaining_source_view.start_column;

                // If token is one of the non-fixed form token types, store the token
                // realisation where we need it.
                if (token.type == TokenType::IDENTIFIER) {
                    // Create identifier entry in global identifier table using view on
                    // token substring.
                    token.identifier_idx = identifier_table.try_insert(
                        remaining_source_view.source.substr(0, token.length)
                    );
                } else if (token.type == TokenType::STRING) {
                    // Put string into string buffer and mark index.
                    token.string_idx = string_table.try_insert(
                        results[0].str().substr(1, token.length - 2)
                    );
                } else if (token.type == TokenType::NUMBER) {
                    // Put string into string buffer and mark index.
                    token.number = mflit::Number(std::move(results[0].str()));
                }

                // Find end line and column of token.
                token.end_line   = token.start_line;
                token.end_column = token.start_column;
                for (auto c : remaining_source_view.source.substr(0, token.length)) {
                    if (c == '\n') {
                        ++token.end_line;
                        token.end_column = 0;
                    } else {
                        ++token.end_column;
                    }
                }

                tokens.emplace_back(token);

                // Don't consider any other possible token matches.
                break;
            }
        }

        mfassert(token.type != TokenType::SENTINEL, "Unidentified Lexical Object");

        // Adjust remaining source view, trimming any whitespace subsequent to last-
        // matched token.
        remaining_source_view.source
            = remaining_source_view.source.substr(token.length);
        remaining_source_view.start_line   = token.end_line;
        remaining_source_view.start_column = token.end_column;

        trim_whitespace(remaining_source_view);
    }
}
