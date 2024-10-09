#ifndef __mattflow_lex_token_h
#define __mattflow_lex_token_h

#include "lex/token_type.h"
#include "literal/identifier.h"
#include "literal/number.h"
#include "literal/string.h"
#include "source.h"

namespace mattflow {
    namespace lex {
        struct Token {
            TokenType type;

            // One required if token is of kind literal.
            mflit::IdentifierIdx identifier_idx;
            mflit::StringIdx     string_idx;
            mflit::Number        number;

            // How will modules work? 1:1 with files?
            // ModuleID module_id;
            FileID   file_id;
            uint64_t length;
            uint64_t start_line, end_line;
            uint64_t start_column, end_column;
        };

        using Tokens = std::vector<Token>;

        bool try_match_token(
            VALIN const mf::SourceView& remaining_source_view,
            VALOUT mflex::Token& token,
            VALOUT mflit::IdentifierTable& identifier_table,
            VALOUT mflit::StringTable& string_table
        );
    }  // namespace lex
}  // namespace mattflow
namespace mflex = mattflow::lex;

#endif  // __mattflow_lex_token_h
