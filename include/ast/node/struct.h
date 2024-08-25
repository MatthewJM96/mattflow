#ifndef __mattflow_ast_node_struct_h
#define __mattflow_ast_node_struct_h

#include "ast/node/node.h"
#include "literal/identifier.h"
#include "type.h"

namespace mattflow {
    namespace ast {
        /**
         * @brief Node reflecting a struct type.
         *
         * @inedge variable
         * @outedges struct field
         */
        struct StructNode : public Node {
            mftype::Type* type;

            StructNode(const StructNode& node) : Node(node) { type = node.type; }

            StructNode(StructNode&& node) : Node(node) { type = node.type; }
        };

        /**
         * @brief Node reflecting a struct field.
         *
         * @inedge variable
         */
        struct StructFieldNode : public Node {
            mflit::IdentifierIdx name;
            mftype::Type*        type;

            StructFieldNode(const StructFieldNode& node) : Node(node) {
                name = node.name;
                type = node.type;
            }

            StructFieldNode(StructFieldNode&& node) : Node(node) {
                name = node.name;
                type = node.type;
            }
        };
    }  // namespace ast
}  // namespace mattflow
namespace mfast = mattflow::ast;

#endif  // __mattflow_ast_node_struct_h
