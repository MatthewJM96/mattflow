#ifndef __mattflow_ast_parse_h
#define __mattflow_ast_parse_h

#include "ast/ast.h"
#include "lex/token.h"

namespace mattflow {
    namespace ast {
        void parse(VALIN const mflex::Tokens& tokens);
    }
}  // namespace mattflow
namespace mfast = mattflow::ast;

#endif  // __mattflow_ast_parse_h
