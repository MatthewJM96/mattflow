#ifndef __mattflow_ast_parse_state_h
#define __mattflow_ast_parse_state_h

#include "ast/ast.h"
#include "ast/node.h"
#include "variable/scope.h"

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
            std::vector<NodeProps>              enclosed_by;
            std::vector<NodeProps>              last_seen;
            std::vector<mfvar::Scope>           scopes;
            std::set<ASTVertex>                 leaf_vertices;
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
         * @param scope_tree The tree of scopes existent in this parsed token stream.
         */
        void push_enclosure(
            VALIN NodeInfo&&    enclosing_node_info,
            NodeProps           enclosing_category,
            VALOUT AST&         ast,
            VALOUT NodeBuffers& nodes,
            VALOUT ParserState& parser_state,
            VALINOUT mfvar::ScopeTree& scope_tree
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
            NodeProps           expected_enclosing_category,
            VALOUT AST&         ast,
            VALOUT NodeBuffers& nodes,
            VALOUT ParserState& parser_state
        );

        /**
         * @brief Pops enclosures from the ParserState stacks up to the first enclosure
         * to be of the target category. Ensures that such pops properly link between
         * the popped enclosures and their parents.
         *
         * @param expected_enclosing_category The target category of the to-be-popped-to
         * enclosure.
         * @param ast The AST to which the enclosures belong.
         * @param nodes The node buffers in which the enclosure nodes will be placed.
         * @param parser_state The ParserState from which the stack entries are popped.
         * @param include Whether to pop the target enclosure as well.
         */
        void pop_enclosures_up_to(
            NodeProps           target_enclosing_category,
            VALOUT AST&         ast,
            VALOUT NodeBuffers& nodes,
            VALOUT ParserState& parser_state,
            bool                include = false
        );

        /**
         * @brief Pops enclosures from the ParserState stacks up to and including the
         * first enclosure to be of the target category. Ensures that such pops properly
         * link between the popped enclosures and their parents.
         *
         * @param expected_enclosing_category The target category of the to-be-popped-to
         * enclosure.
         * @param ast The AST to which the enclosures belong.
         * @param nodes The node buffers in which the enclosure nodes will be placed.
         * @param parser_state The ParserState from which the stack entries are popped.
         */
        void pop_enclosures_up_to_and_including(
            NodeProps           target_enclosing_category,
            VALOUT AST&         ast,
            VALOUT NodeBuffers& nodes,
            VALOUT ParserState& parser_state
        );
    }  // namespace ast
}  // namespace mattflow
namespace mfast = mattflow::ast;

#endif  // __mattflow_ast_parse_state_h
