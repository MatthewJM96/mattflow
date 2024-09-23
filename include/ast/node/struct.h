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
            StructNode(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token,
                mftype::Type*                 _type
            ) :
                Node(_first_token, _last_token), type(_type) {
                // Empty.
            }

            mftype::Type* type;

            std::string debug_repr() override { return "struct"; }
        };

        /**
         * @brief Node reflecting a struct field.
         *
         * @inedge variable
         */
        struct StructFieldNode : public Node {
            StructFieldNode(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token,
                mflit::IdentifierIdx          _name,
                mftype::Type*                 _type
            ) :
                Node(_first_token, _last_token), name(_name), type(_type) {
                // Empty.
            }

            mflit::IdentifierIdx name;
            mftype::Type*        type;

            std::string debug_repr() override { return "struct field"; }
        };
    }  // namespace ast
}  // namespace mattflow
namespace mfast = mattflow::ast;

#endif  // __mattflow_ast_node_struct_h
