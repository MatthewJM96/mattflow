#ifndef __mattflow_ast_link_h
#define __mattflow_ast_link_h

#include "ast/parse.h"

namespace mattflow {
    namespace ast {
        void link_operations_on_stack(
            VALIN mfast::Precedence target_precedence,
            VALOUT mfast::AST& ast,
            VALOUT mfast::NodeBuffers& nodes,
            VALOUT mfast::ParserState& parser_state
        );
    }  // namespace ast
}  // namespace mattflow
namespace mfast = mattflow::ast;

#endif  // __mattflow_ast_link_h
