#ifndef __mattflow_lex_token_h
#define __mattflow_lex_token_h

#include "identifier.h"

namespace mattflow {
    namespace lex {
        struct Token {
            TokenType type;

            // Required if token is of kind literal.
            IdentifierIdx identifier_idx;

            uint64_t line, first_column;
        };
    }  // namespace lex
}  // namespace mattflow
namespace mflex = mattflow::lex;

#endif  // __mattflow_lex_token_h
