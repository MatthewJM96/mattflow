#ifndef __mattflow_ast_parse_link_h
#define __mattflow_ast_parse_link_h

#include "ast/parse/parse.h"
#include "ast/parse/state.h"

namespace mattflow {
    namespace ast {
        /**
         * @brief Link operations on the top stack of ParserState if there is an end of
         * expression. The assumption is a non-operating node is about to be added, so
         * the existence of a non-operating last-seen node is the condition checked.
         *
         * @param ast The AST to which the operations belong.
         * @param nodes The node buffers in which node information is stored.
         * @param parser_state The ParserState on which the to-be-linked operators lie.
         */
        void maybe_link_operations_on_stack(
            VALOUT mfast::AST& ast,
            VALOUT mfast::NodeBuffers& nodes,
            VALOUT mfast::ParserState& parser_state
        );

        /**
         * @brief Link operations on the top stack of ParserState.
         *
         * @param target_precedence The precedence at which to stop linking operations.
         * @param ast The AST to which the operations belong.
         * @param nodes The node buffers in which node information is stored.
         * @param parser_state The ParserState on which the to-be-linked operators lie.
         */
        void link_operations_on_stack(
            VALIN mfast::Precedence target_precedence,
            VALOUT mfast::AST& ast,
            VALOUT mfast::NodeBuffers& nodes,
            VALOUT mfast::ParserState& parser_state
        );
    }  // namespace ast
}  // namespace mattflow
namespace mfast = mattflow::ast;

#endif  // __mattflow_ast_parse_link_h
