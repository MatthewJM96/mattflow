#ifndef __mattflow_variable_variable_h
#define __mattflow_variable_variable_h

#include "ast/parse/state.h"
#include "type/type.h"

namespace mattflow {
    namespace variable {
        class VariableTypeTable {
        public:
            using Map         = std::unordered_map<mflit::IdentifierIdx, mftype::Type>;
            using ScopeMap    = std::unordered_map<Scope, Map>;
            using MapIterator = Map::const_iterator;
            using MapEntry    = std::pair<MapIterator, bool>;

            VariableTypeTable() {
                // Empty.
            }

            MATTFLOW_NON_COPYABLE(VariableTypeTable);

            MapEntry try_insert(Scope scope, mflit::IdentifierIdx identifier);
            MapEntry try_insert(
                Scope scope, mflit::IdentifierIdx identifier, const mftype::Type& type
            );

            MapEntry associate_type(
                Scope scope, mflit::IdentifierIdx identifier, const mftype::Type& type
            );
            MapEntry associate_type_of_identifier(
                Scope                scope,
                mflit::IdentifierIdx identifier,
                mflit::IdentifierIdx type_of_identifier
            );
            MapEntry associate_type_held_by_identifier(
                Scope                scope,
                mflit::IdentifierIdx identifier,
                mflit::IdentifierIdx type_held_by_identifier
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
                Scope                       scope,
                const ScopeTree&            scope_tree,
                const mflit::IdentifierIdx& identifier
            ) const;

            const Map* get_scope_map(Scope scope) const;
        protected:
            Map& try_insert_scope_map(Scope scope);

            ScopeMap m_scope_var_type_map;
        };
    }  // namespace variable
}  // namespace mattflow
namespace mfvar = mattflow::variable;

#endif  // __mattflow_variable_variable_h
