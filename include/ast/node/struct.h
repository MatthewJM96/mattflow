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
         */
        struct StructNode : public Node {
            mflit::IdentifierIdx              name;
            mftype::Type*                     type;
            std::vector<mflit::IdentifierIdx> field_names;
        };
    }  // namespace ast
}  // namespace mattflow
namespace mfast = mattflow::ast;

#endif  // __mattflow_ast_node_struct_h
