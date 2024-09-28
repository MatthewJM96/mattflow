#ifndef __mattflow_ast_node_function_h
#define __mattflow_ast_node_function_h

#include "../../type.h"
#include "ast/node/node.h"
#include "literal/identifier.h"

namespace mattflow {
    namespace ast {
        /**
         * @brief Node reflecting a function definition.
         *
         * @inedge operator | paren expression | assignment | block expression
         * @outedge rvalue expression
         */
        struct FunctionNode : public Node {
            FunctionNode(
                mflex::Tokens::const_iterator       _first_token,
                mflex::Tokens::const_iterator       _last_token,
                mftype::Type*                       _type,
                std::vector<mflit::IdentifierIdx>&& _argument_names
            ) :
                Node(_first_token, _last_token),
                type(_type),
                argument_names(
                    std::forward<std::vector<mflit::IdentifierIdx>>(_argument_names)
                ) {
                // Empty.
            }

            virtual ~FunctionNode() {
                // Empty.
            }

            mftype::Type*                     type;
            std::vector<mflit::IdentifierIdx> argument_names;

            std::string debug_repr() const override { return "func"; }
        };
    }  // namespace ast
}  // namespace mattflow
namespace mfast = mattflow::ast;

#endif  // __mattflow_ast_node_function_h
