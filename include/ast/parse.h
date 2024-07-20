#ifndef __mattflow_ast_parse_h
#define __mattflow_ast_parse_h

#include "ast/ast.h"
#include "lex/token.h"

namespace mattflow {
    namespace ast {
        // TODO(Matthew): work out what will be useful here, for now put some
        //                suggestions in as placeholders.
        namespace parser_expects {
            enum ParserExpectsFlag : uint16_t {
                EXPRESSION = 0,
                ASSIGNMENT = 100,
                LOGIC      = 200,
                EQUALITY   = 300,
                COMPARISON = 400,
                TERM       = 500,
                FACTOR     = 600,
                UNARY      = 700,
                CALL       = 800,
                PRIMARY    = 900
            };
        }  // namespace parser_expects

        enum class Associativity {
            ANY,
            LEFT,
            RIGHT
        };

        struct ParserState {
            size_t         cursor;
            uint16_t*      precedence;
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
