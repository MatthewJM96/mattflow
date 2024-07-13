#ifndef __mattflow_ast_ast_h
#define __mattflow_ast_ast_h

namespace mattflow {
    namespace ast {
        enum vertex_data_t {
            vertex_data
        };
    }  // namespace ast
}  // namespace mattflow
namespace mfast = mattflow::ast;

namespace boost {
    template <>
    struct property_kind<mfast::vertex_data_t> {
        typedef vertex_property_tag type;
    };
}  // namespace boost

namespace mattflow {
    namespace ast {
        struct Node;

        using AST = boost::adjacency_list<
            boost::vecS,
            boost::vecS,
            boost::directedS,
            boost::property<vertex_data_t, Node*>>;
    }  // namespace ast
}  // namespace mattflow
namespace mfast = mattflow::ast;

#endif  // __mattflow_ast_ast_h
