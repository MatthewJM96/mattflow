#ifndef __mattflow_variable_scope_h
#define __mattflow_variable_scope_h

namespace mattflow {
    namespace variable {
        using ScopeTree
            = boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS>;

        using Scope = ScopeTree::vertex_descriptor;
    }  // namespace variable
}  // namespace mattflow
namespace mfvar = mattflow::variable;

#endif  // __mattflow_variable_scope_h
