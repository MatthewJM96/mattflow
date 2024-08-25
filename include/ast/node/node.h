#ifndef __mattflow_ast_node_node_h
#define __mattflow_ast_node_node_h

#include "lex/token.h"

namespace mattflow {
    namespace ast {
        struct Node {
            mflex::Tokens::const_iterator first_token, last_token;

            Node(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) {
                first_token = _first_token;
                last_token  = _last_token;
            }

            Node(const Node& node) {
                first_token = node.first_token;
                last_token  = node.last_token;
            }

            Node(Node&& node) {
                first_token = node.first_token;
                last_token  = node.last_token;
            }
        };
    }  // namespace ast
}  // namespace mattflow
namespace mfast = mattflow::ast;

#endif  // __mattflow_ast_node_node_h
