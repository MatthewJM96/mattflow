#ifndef __mattflow_ast_node_node_h
#define __mattflow_ast_node_node_h

#include "lex/token.h"

namespace mattflow {
    namespace ast {
        struct Node {
            Node(
                mflex::Tokens::const_iterator _first_token,
                mflex::Tokens::const_iterator _last_token
            ) :
                first_token(_first_token), last_token(_last_token) {
                // Empty.
            }

            mflex::Tokens::const_iterator first_token, last_token;

            virtual std::string debug_repr() = 0;
        };
    }  // namespace ast
}  // namespace mattflow
namespace mfast = mattflow::ast;

#endif  // __mattflow_ast_node_node_h
