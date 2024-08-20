#ifndef __mattflow_ast_parse_h
#define __mattflow_ast_parse_h

#include "ast/ast.h"
#include "lex/token.h"

namespace mattflow {
    namespace ast {
        void parse(
            VALIN const mflex::Tokens& tokens,
            VALOUT AST&                ast,
            VALOUT NodeBuffers&        nodes,
            VALOUT mftype::TypeTable& type_table
        );
    }  // namespace ast
}  // namespace mattflow
namespace mfast = mattflow::ast;

#endif  // __mattflow_ast_parse_h
