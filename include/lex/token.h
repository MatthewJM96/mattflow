#ifndef __mattflow_lex_token_h
#define __mattflow_lex_token_h

#include "identifier.h"
#include "lex/token_type.h"
#include "number.h"

namespace mattflow {
    namespace lex {
        struct Token {
            TokenType type;

            // One required if token is of kind literal.
            IdentifierIdx identifier_idx;
            std::string   string;
            Number        number;

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
