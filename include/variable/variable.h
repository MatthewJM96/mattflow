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
            MapEntry associate_type(
                mfvar::Scope         scope,
                mflit::IdentifierIdx identifier,
                mflit::IdentifierIdx type
            );

            /**
             * @brief Find the type of a variable, walking up the scope tree if
             * necessary.
             *
             * @param scope The current scope to start searching from.
             * @param scope_tree The scope tree to walk.
             * @param identifier The identifier of the variable to find.
             * @return const mftype::Type* The type if found, otherwise nullptr.
             */
            const mftype::Type* find(
                mfvar::Scope                scope,
                const mfvar::ScopeTree&     scope_tree,
                const mflit::IdentifierIdx& identifier
            ) const;
        protected:
            const Map* get_scope_map(mfvar::Scope scope) const;
            Map&       get_scope_map(mfvar::Scope scope);

            ScopeMap m_scope_var_type_map;
        };
    }  // namespace variable
}  // namespace mattflow
namespace mfvar = mattflow::variable;

#endif  // __mattflow_variable_variable_h
