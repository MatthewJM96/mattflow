#ifndef __mattflow_ast_node_variable_h
#define __mattflow_ast_node_variable_h

#include "ast/node/node.h"
#include "type.h"

namespace mattflow {
    namespace ast {
        /**
         * @brief Node reflecting a variable.
         *
         * @inedge block expression
         * @outedge type expression
         * @outedge rvalue expression
         */
        struct VariableNode : public Node {
            mflit::IdentifierIdx name;
        };

        /**
         * @brief Node reflecting a value assignment.
         *
         * @inedge lvalue expression
         * @outedge rvalue expression
         */
        struct AssignmentNode : public Node {
            // in_edge: lvalue expression (e.g. variable, struct field, etc)
            // out_edge: rvalue expression (e.g. function call, arithmetic, etc)

            // Empty.
        };

        // TODO(Matthew): change this down the line to enable scoping.
        using VariableTable = std::unordered_map<mflit::IdentifierIdx, mftype::Type*>;
    }  // namespace ast
}  // namespace mattflow
namespace mfast = mattflow::ast;

#endif  // __mattflow_ast_node_variable_h
