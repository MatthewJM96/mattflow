#ifndef __mattflow_ast_node_function_h
#define __mattflow_ast_node_function_h

#include "ast/node/node.h"
#include "literal/identifier.h"
#include "type.h"

namespace mattflow {
    namespace ast {
        /**
         * @brief Node reflecting a function definition.
         *
         * @inedge operator | paren expression | assignment | block expression
         * @outedge rvalue expression
         */
        struct FunctionNode : public Node {
            mftype::Type*                     type;
            std::vector<mflit::IdentifierIdx> argument_names;

            FunctionNode(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) :
                Node(_first_token, _last_token) { }

            FunctionNode(const FunctionNode& node) : Node(node) {
                type           = node.type;
                argument_names = node.argument_names;
            }

            FunctionNode(FunctionNode&& node) : Node(node) {
                type           = node.type;
                argument_names = std::move(node.argument_names);
            }
        };
    }  // namespace ast
}  // namespace mattflow
namespace mfast = mattflow::ast;

#endif  // __mattflow_ast_node_function_h
