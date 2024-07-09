#ifndef __mattflow_lex_token_h
#define __mattflow_lex_token_h

#include "lex/token_type.h"
#include "literal/identifier.h"
#include "literal/number.h"
#include "source.h"

namespace mattflow {
    namespace lex {
        struct Token {
            TokenType type;

            // One required if token is of kind literal.
            mflit::IdentifierIdx identifier_idx;
            std::string          string;
            mflit::Number        number;

            // How will modules work? 1:1 with files?
            // ModuleID module_id;
            FileID   file_id;
            uint64_t length;
            uint64_t start_line, end_line;
            uint64_t start_column, end_column;
        };

        using Tokens = std::vector<Token>;
    }  // namespace lex
}  // namespace mattflow
namespace mflex = mattflow::lex;

#endif  // __mattflow_lex_token_h
