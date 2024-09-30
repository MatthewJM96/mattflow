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
        for (auto& matcher : TOKEN_MATCHERS) {
            bool   found_match  = false;
            size_t match_length = 0;

            if (matcher.matching_strategy == TokenMatchingStrategy::EXACT
                || matcher.matching_strategy == TokenMatchingStrategy::KEYWORD)
            {
                // Try to match current pattern exactly.
                //   NOTE: reinterpret_cast needed to satisfy MSVC.
                found_match = remaining_source_view.source.starts_with(matcher.pattern);

                if (matcher.matching_strategy == TokenMatchingStrategy::KEYWORD) {
                    // Make sure the keyword is followed by one of the valid stopwords.
                    char next_char
                        = *(remaining_source_view.source.data()
                            + matcher.pattern.length());
                    bool found_stopword = false;
                    for (char stopword : mflex::KEYWORD_STOPWORDS) {
                        if (next_char == stopword) {
                            found_stopword = true;
                            break;
                        }
                    }

                    found_match = found_match && found_stopword;
                }

                match_length = matcher.pattern.length();
            } else if (matcher.matching_strategy == TokenMatchingStrategy::IDENTIFIER) {
                size_t curr_char
                    = static_cast<size_t>(*remaining_source_view.source.data());
                size_t offset = 0;

                if ((static_cast<size_t>('a') <= curr_char
                     && curr_char <= static_cast<size_t>('z'))
                    || (static_cast<size_t>('A') <= curr_char
                        && curr_char <= static_cast<size_t>('Z'))
                    || static_cast<size_t>('_') == curr_char)
                {
                    do {
                        curr_char = *(remaining_source_view.source.data() + ++offset);
                    } while ((static_cast<size_t>('a') <= curr_char
                              && curr_char <= static_cast<size_t>('z'))
                             || (static_cast<size_t>('A') <= curr_char
                                 && curr_char <= static_cast<size_t>('Z'))
                             || (static_cast<size_t>('0') <= curr_char
                                 && curr_char <= static_cast<size_t>('9'))
                             || static_cast<size_t>('_') == curr_char);
                }

                if (offset > 0) {
                    found_match  = true;
                    match_length = offset;

                    token.identifier_idx = identifier_table.try_insert(
                        remaining_source_view.source.substr(0, match_length)
                    );
                }
            } else if (matcher.matching_strategy == TokenMatchingStrategy::STRING) {
                size_t offset          = 0;
                size_t internal_offset = 0;

                if (remaining_source_view.source.starts_with("\"\"\"")) {
                    offset          = 2;
                    internal_offset = 3;
                    while (++offset < remaining_source_view.source.length()) {
                        if (remaining_source_view.source.substr(offset).starts_with(
                                "\"\"\""
                            ))
                        {
                            found_match = true;
                            break;
                        }
                    }
                } else if (remaining_source_view.source.starts_with("\"")) {
                    offset          = 0;
                    internal_offset = 1;
                    while (++offset < remaining_source_view.source.length()) {
                        if (remaining_source_view.source.substr(offset).starts_with("\""
                            ))
                        {
                            found_match = true;
                            break;
                        }
                    }
                }

                if (found_match) {
                    match_length     = offset + internal_offset;
                    token.string_idx = string_table.try_insert(
                        std::string(remaining_source_view.source.substr(
                            internal_offset, match_length - 2 * internal_offset
                        ))
                    );
                }
            } else if (matcher.matching_strategy == TokenMatchingStrategy::NUMBER) {
                size_t curr_char
                    = static_cast<size_t>(*remaining_source_view.source.data());
                size_t offset = 0;

                while (static_cast<size_t>('0') <= curr_char
                       && curr_char <= static_cast<size_t>('9'))
                {
                    curr_char = *(remaining_source_view.source.data() + ++offset);
                }

                if (offset == 0) {
                    found_match = false;
                    goto end_matching;
                }

                found_match = true;

                if (curr_char == '.') {
                    ++offset;
                } else {
                    goto scientific_notation;
                }

                do {
                    curr_char = *(remaining_source_view.source.data() + ++offset);
                } while (static_cast<size_t>('0') <= curr_char
                         && curr_char <= static_cast<size_t>('9'));

scientific_notation:

                if (curr_char == 'd' || curr_char == 'D' || curr_char == 'e'
                    || curr_char == 'E')
                {
                    ++offset;
                } else {
                    match_length = offset;
                    goto end_matching;
                }

                do {
                    curr_char = *(remaining_source_view.source.data() + ++offset);
                } while (static_cast<size_t>('0') <= curr_char
                         && curr_char <= static_cast<size_t>('9'));

                match_length = offset;
end_matching:
                if (found_match) {
                    token.number = mflit::Number(
                        remaining_source_view.source.substr(0, match_length)
                    );
                }
            }

            // Construct token if we matched.
            if (found_match) {
                token.type    = matcher.type;
                token.file_id = remaining_source_view.file_id;

                token.length       = match_length;
                token.start_line   = remaining_source_view.start_line;
                token.start_column = remaining_source_view.start_column;

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

                // Don't consider any other patterns.
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
