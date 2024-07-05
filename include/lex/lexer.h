#ifndef __mattflow_lex_lexer_h
#define __mattflow_lex_lexer_h

#include "lex/token.h"
#include "source.h"

namespace mattflow {
    namespace lex {
        void parse(SourceView source_view, VALOUT Tokens& tokens);
    }
}  // namespace mattflow
namespace mflex = mattflow::lex;

#endif  // __mattflow_lex_lexer_h
