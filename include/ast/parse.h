#ifndef __mattflow_ast_parse_h
#define __mattflow_ast_parse_h

#include "ast/ast.h"
#include "ast/node.h"
#include "lex/token.h"

namespace mattflow {
    namespace ast {
        /**
         * @brief Holds state for a parse call. The vertices buffer is built up
         * per-node while the precedence and associativity are built up per layer
         * whether block expression, parantheses expression, etc.
         */
        struct ParserState {
            std::vector<Precedence>        precedence;
            std::vector<Associativity>     associativity;
            std::vector<ASTVertex>         vertices;
            std::vector<EnclosingCategory> enclosed_by;
            std::vector<NodeCategory>      last_seen;
        };

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
