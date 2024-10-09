#include "stdafx.h"

#include "lex/token.h"

static bool try_exact_match(
    VALIN const mflex::TokenMatcher& matcher,
    VALIN const mf::SourceView& remaining_source_view,
    VALOUT size_t&              match_length,
    bool                        is_keyword
) {
    match_length = matcher.pattern.length();

    // Try to match current pattern exactly.
    //   NOTE: reinterpret_cast needed to satisfy MSVC.
    if (!remaining_source_view.source.starts_with(matcher.pattern)) {
        return false;
    }

    if (is_keyword) {
        // Make sure the keyword is followed by one of the valid stopwords.
        char next_char
            = *(remaining_source_view.source.data() + matcher.pattern.length());
        for (char stopword : mflex::KEYWORD_STOPWORDS) {
            if (next_char == stopword) return true;
        }

        return false;
    }

    return true;
}

static bool try_identifier_match(
    VALIN const mf::SourceView& remaining_source_view,
    VALOUT mflex::Token& token,
    VALOUT mflit::IdentifierTable& identifier_table,
    VALOUT size_t&                 match_length
) {
    size_t curr_char = static_cast<size_t>(*remaining_source_view.source.data());
    size_t offset    = 0;

    if ((static_cast<size_t>('a') <= curr_char && curr_char <= static_cast<size_t>('z'))
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
        match_length = offset;

        token.identifier_idx = identifier_table.try_insert(
            remaining_source_view.source.substr(0, match_length)
        );

        return true;
    }

    return false;
}

static bool try_string_match(
    VALIN const mf::SourceView& remaining_source_view,
    VALOUT mflex::Token& token,
    VALOUT mflit::StringTable& string_table,
    VALOUT size_t&             match_length
) {
    bool   string_started  = false;
    bool   found_match     = false;
    size_t offset          = 0;
    size_t internal_offset = 0;

    if (remaining_source_view.source.starts_with("\"\"\"")) {
        string_started  = true;
        offset          = 2;
        internal_offset = 3;
        while (++offset < remaining_source_view.source.length()) {
            if (remaining_source_view.source.substr(offset).starts_with("\"\"\"")) {
                found_match = true;
                break;
            }
        }
    } else if (remaining_source_view.source.starts_with("\"")) {
        string_started  = true;
        offset          = 0;
        internal_offset = 1;
        while (++offset < remaining_source_view.source.length()) {
            if (remaining_source_view.source.substr(offset).starts_with("\"")) {
                found_match = true;
                break;
            }
        }
    }

    mfassert(
        (string_started && found_match) || !found_match, "String opened but not closed."
    );

    if (found_match) {
        match_length = offset + internal_offset;
        token.string_idx
            = string_table.try_insert(std::string(remaining_source_view.source.substr(
                internal_offset, match_length - 2 * internal_offset
            )));
    }

    return found_match;
}

static bool try_number_match(
    VALIN const mf::SourceView& remaining_source_view,
    VALOUT mflex::Token& token,
    VALOUT size_t&       match_length
) {
    size_t curr_char = static_cast<size_t>(*remaining_source_view.source.data());
    size_t offset    = 0;

    ////////////////////////////////////////////////////////////////////////////////////
    // First digits

    while (static_cast<size_t>('0') <= curr_char
           && curr_char <= static_cast<size_t>('9'))
    {
        curr_char = *(remaining_source_view.source.data() + ++offset);
    }

    // No digits, no number.
    if (offset == 0) return false;

    // If we just have digits, then great, we have a valid integer.
    if (curr_char == ' ' || curr_char == '\n') goto found_match;

    ////////////////////////////////////////////////////////////////////////////////////
    // Decimal Maybe

    // If not a decimal, maybe scientific notation?
    if (curr_char != '.') goto scientific_notation;

    curr_char = *(remaining_source_view.source.data() + ++offset);

    // We have digits and a decimal point, we have a valid float.
    if (curr_char == ' ' || curr_char == '\n') goto found_match;

    // Count any digits following the decimal point, if any.
    while (static_cast<size_t>('0') <= curr_char
           && curr_char <= static_cast<size_t>('9'))
    {
        curr_char = *(remaining_source_view.source.data() + ++offset);
    }

scientific_notation:

    if (curr_char == 'd' || curr_char == 'D' || curr_char == 'e' || curr_char == 'E') {
        curr_char = *(remaining_source_view.source.data() + ++offset);

        mfassert(
            static_cast<size_t>('0') <= curr_char
                && curr_char <= static_cast<size_t>('9'),
            "Had a number that looked like scientific notation but no digits "
            " following\nthe exponent indicator."
        );

        while (static_cast<size_t>('0') <= curr_char
               && curr_char <= static_cast<size_t>('9'))
        {
            curr_char = *(remaining_source_view.source.data() + ++offset);
        }
    }

found_match:
    match_length = offset;
    token.number = mflit::Number(remaining_source_view.source.substr(0, match_length));
    return true;
}

bool mflex::try_match_token(
    VALIN const mf::SourceView& remaining_source_view,
    VALOUT Token&               token,
    VALOUT mflit::IdentifierTable& identifier_table,
    VALOUT mflit::StringTable& string_table
) {
    size_t match_length = 0;

    auto construct_token = [&](TokenType token_type) {
        token.type    = token_type;
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
    };

    for (auto& matcher : EXACT_TOKEN_MATCHERS) {
        if (try_exact_match(matcher, remaining_source_view, match_length, false)) {
            construct_token(matcher.type);

            return true;
        }
    }

    for (auto& matcher : KEYWORD_TOKEN_MATCHERS) {
        if (try_exact_match(matcher, remaining_source_view, match_length, true)) {
            construct_token(matcher.type);

            return true;
        }
    }

    if (try_identifier_match(
            remaining_source_view, token, identifier_table, match_length
        ))
    {
        construct_token(TokenType::IDENTIFIER);

        return true;
    }

    if (try_string_match(remaining_source_view, token, string_table, match_length)) {
        construct_token(TokenType::STRING);

        return true;
    }

    if (try_number_match(remaining_source_view, token, match_length)) {
        construct_token(TokenType::NUMBER);

        return true;
    }

    return false;
}
