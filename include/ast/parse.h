#ifndef __mattflow_ast_parse_h
#define __mattflow_ast_parse_h

#include "ast/ast.h"
#include "lex/token.h"

namespace mattflow {
    namespace ast {
        // TODO(Matthew): work out what will be useful here, for now put some
        //                suggestions in as placeholders.
        namespace parser_expects {
            enum ParserExpectsFlag {
                IDENTIFIER    = 0x000,
                NUMBER        = 0x001,
                LEFT_PAREN    = 0x002,
                RIGHT_PAREN   = 0x004,
                LEFT_BRACE    = 0x008,
                RIGHT_BRACE   = 0x010,
                LEFT_BRACKET  = 0x020,
                RIGHT_BRACKET = 0x040
            };

            const auto ALL
                = ParserExpectsFlag::IDENTIFIER | ParserExpectsFlag::NUMBER
                  | ParserExpectsFlag::LEFT_PAREN | ParserExpectsFlag::RIGHT_PAREN
                  | ParserExpectsFlag::LEFT_BRACE | ParserExpectsFlag::RIGHT_BRACE
                  | ParserExpectsFlag::LEFT_BRACKET | ParserExpectsFlag::RIGHT_BRACKET;
        }  // namespace parser_expects

        enum class Associativity {
            ANY,
            LEFT,
            RIGHT
        };

        struct ParserState {
            size_t         cursor;
            int*           expects;
            int*           precedence;
            Associativity* associativity;
            ASTVertex*     vertex;
        };

        void parse(
            VALIN const mflex::Tokens& tokens,
            VALOUT AST&                ast,
            VALOUT NodeBuffers&        nodes,
            VALOUT VariableTable&      variable_table,
            VALOUT mftype::TypeTable& type_table
        );
    }  // namespace ast
}  // namespace mattflow
namespace mfast = mattflow::ast;

#endif  // __mattflow_ast_parse_h
