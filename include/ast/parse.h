#ifndef __mattflow_ast_parse_h
#define __mattflow_ast_parse_h

#include "ast/ast.h"
#include "lex/token.h"

namespace mattflow {
    namespace ast {
        // TODO(Matthew): work out what will be useful here, for now put some
        //                suggestions in as placeholders.
        enum class ParserExpectsFlag {
            IDENTIFIER    = 0x000,
            NUMBER        = 0x001,
            LEFT_PAREN    = 0x002,
            RIGHT_PAREN   = 0x004,
            LEFT_BRACE    = 0x008,
            RIGHT_BRACE   = 0x010,
            LEFT_BRACKET  = 0x020,
            RIGHT_BRACKET = 0x040
        };

        void parse(
            VALIN const mflex::Tokens& tokens,
            VALOUT AST&                ast,
            VALOUT NodeBuffers&        nodes
        );
    }  // namespace ast
}  // namespace mattflow
namespace mfast = mattflow::ast;

#endif  // __mattflow_ast_parse_h
