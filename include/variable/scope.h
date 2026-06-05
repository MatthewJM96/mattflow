#ifndef __mattflow_variable_scope_h
#define __mattflow_variable_scope_h

namespace mattflow {
    namespace variable {
        class VariableTypeTable;

        using ScopeTree
            = boost::adjacency_list<boost::vecS, boost::vecS, boost::bidirectionalS>;

        using Scope = ScopeTree::vertex_descriptor;

        class ScopeTreeNodeInfoWriter {
        public:
            ScopeTreeNodeInfoWriter(VariableTypeTable* _var_table) :
                var_table(_var_table) { }

            void operator()(std::ostream& out, Scope scope) const;
        private:
            VariableTypeTable* var_table;
        };
    }  // namespace variable
}  // namespace mattflow
namespace mfvar = mattflow::variable;

#endif  // __mattflow_variable_scope_h
