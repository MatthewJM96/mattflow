#ifndef __mattflow_ast_node_properties_h
#define __mattflow_ast_node_properties_h

namespace mattflow {
    namespace ast {
        using Precedence                = int16_t;
        const Precedence NO_PRECENDENCE = -1;

        enum class Associativity {
            NONE,
            LEFT,
            RIGHT
        };

        template <typename T>
        struct NodeProperties {
            Precedence    precedence;
            Associativity associativity;
        };
    }  // namespace ast
}  // namespace mattflow
namespace mfast = mattflow::ast;

#endif  // __mattflow_ast_node_properties_h
