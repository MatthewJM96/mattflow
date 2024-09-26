#ifndef __mattflow_ast_state_h
#define __mattflow_ast_state_h

#include "ast/ast.h"
#include "ast/node.h"

namespace mattflow {
    namespace ast {
        /**
         * @brief Holds state for a parse call. The vertices buffer is built up
         * per-node while the precedence and associativity are built up per layer
         * whether block expression, parantheses expression, etc.
         */
        struct ParserState {
            std::vector<Precedence>             precedence;
            std::vector<Associativity>          associativity;
            std::vector<std::vector<ASTVertex>> operating_vertices;
            std::vector<std::vector<ASTVertex>> non_operating_vertices;
            std::vector<ASTVertex>              enclosing_vertices;
            std::vector<EnclosingCategory>      enclosed_by;
            std::vector<NodeCategory>           last_seen;
        };

        /**
         * @brief Pushes an enclosure onto the ParserState stacks. Ensures that such
         * push properly enables linking between the new enclosure and its parent.
         *
         * @param enclosing_node_info The NodeInfo block for the enclosing vertex.
         * @param enclosing_category The category of the enclosure.
         * @param ast The AST to which the enclosure belongs.
         * @param nodes The node buffers in which the enclosure node will be placed.
         * @param parser_state The ParserState on which the new stack entries are
         * pushed.
         */
        void push_enclosure(
            VALIN mfast::NodeInfo&&  enclosing_node_info,
            mfast::EnclosingCategory enclosing_category,
            VALOUT mfast::AST&  ast,
            VALOUT NodeBuffers& nodes,
            VALOUT ParserState& parser_state
        );

        /**
         * @brief Pops an enclosure from the ParserState stacks. Ensures that such
         * pop properly links between the popped enclosure and its parent.
         *
         * @param expected_enclosing_category The expected category of the to-be-popped
         * enclosure.
         * @param ast The AST to which the enclosure belongs.
         * @param nodes The node buffers in which the enclosure node will be placed.
         * @param parser_state The ParserState from which the stack entries are popped.
         */
        void pop_enclosure(
            mfast::EnclosingCategory expected_enclosing_category,
            VALOUT mfast::AST&  ast,
            VALOUT NodeBuffers& nodes,
            VALOUT ParserState& parser_state
        );
    }  // namespace ast
}  // namespace mattflow
namespace mfast = mattflow::ast;

#endif  // __mattflow_ast_state_h
