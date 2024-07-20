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
                EXPRESSION,
                ASSIGNMENT,
                LOGIC,
                EQUALITY,
                COMPARISON,
                TERM,
                FACTOR,
                UNARY,
                CALL,
                PRIMARY
            };
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
