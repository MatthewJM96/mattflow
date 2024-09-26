#ifndef __mattflow_ast_parse_h
#define __mattflow_ast_parse_h

#include "ast/ast.h"
#include "ast/node.h"
#include "lex/token.h"

namespace mattflow {
    namespace ast {
        /**
         * @brief Entry point to begin parsing a token stream. Produces an AST,
         * populating the provided node buffers and the type table.
         *
         * @param tokens The token stream to parse.
         * @param ast The resulting AST after parsing.
         * @param nodes The node buffers which are populated with information about each
         * node added to the AST.
         * @param type_table The type table in which any non-primitive types are
         * recorded, with the information so far gathered about those types.
         */
        void parse(
            VALIN const mflex::Tokens& tokens,
            VALOUT AST&                ast,
            VALOUT NodeBuffers&        nodes,
            VALOUT mftype::IdentifierTypeTable& type_table
        );
    }  // namespace ast
}  // namespace mattflow
namespace mfast = mattflow::ast;

#endif  // __mattflow_ast_parse_h
