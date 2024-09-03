#ifndef __mattflow_ast_node_node_h
#define __mattflow_ast_node_node_h

#include "lex/token.h"

namespace mattflow {
    namespace ast {
        struct Node {
            mflex::Tokens::const_iterator first_token, last_token;
        };
    }  // namespace ast
}  // namespace mattflow
namespace mfast = mattflow::ast;

#endif  // __mattflow_ast_node_node_h
