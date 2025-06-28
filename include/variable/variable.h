#ifndef __mattflow_variable_variable_h
#define __mattflow_variable_variable_h

#include "ast/parse/state.h"
#include "type/type.h"

namespace mattflow {
    namespace variable {
        class VariableTypeTable {
        public:
            using Map         = std::unordered_map<mflit::IdentifierIdx, mftype::Type>;
            using ScopeMap    = std::unordered_map<mfvar::Scope, Map>;
            using MapIterator = Map::const_iterator;
            using MapEntry    = std::pair<MapIterator, bool>;

            VariableTypeTable() {
                // Empty.
            }

            MATTFLOW_NON_COPYABLE(VariableTypeTable);

            MapEntry try_insert(mfvar::Scope scope, mflit::IdentifierIdx identifier);
            MapEntry try_insert(
                mfvar::Scope         scope,
                mflit::IdentifierIdx identifier,
                const mftype::Type&  type
            );

            MapEntry associate_type(
                mfvar::Scope         scope,
                mflit::IdentifierIdx identifier,
                const mftype::Type&  type
            );
        protected:
            Map& get_scope_map(mfvar::Scope scope);

            ScopeMap m_scope_var_type_map;
        };
    }  // namespace variable
}  // namespace mattflow
namespace mfvar = mattflow::variable;

#endif  // __mattflow_variable_variable_h
