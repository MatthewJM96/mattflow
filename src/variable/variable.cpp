#include "stdafx.h"

#include "type/type.h"

#include "variable/variable.h"

mfvar::VariableTypeTable::MapEntry mfvar::VariableTypeTable::try_insert(
    mfvar::Scope scope, mflit::IdentifierIdx identifier
) {
    auto& scope_map = try_insert_scope_map(scope);
    auto  it        = scope_map.find(identifier);

    if (it == scope_map.end()) {
        return scope_map.insert({ identifier, mftype::UnresolvedType{} });
    }

    return { it, false };
}

mfvar::VariableTypeTable::MapEntry mfvar::VariableTypeTable::try_insert(
    mfvar::Scope scope, mflit::IdentifierIdx identifier, const mftype::Type& type
) {
    auto& scope_map = try_insert_scope_map(scope);
    auto  it        = scope_map.find(identifier);

    if (it == scope_map.end()) {
        return scope_map.insert({ identifier, type });
    }

    return { it, false };
}

mfvar::VariableTypeTable::MapEntry mfvar::VariableTypeTable::associate_type(
    mfvar::Scope scope, mflit::IdentifierIdx identifier, const mftype::Type& type
) {
    // TODO(Matthew): Type association may need to allow for walking up scope tree
    //                to find where identifier was declared. This is only unnecessary
    //                if in every case we have new info to inform type association we
    //                also know the exact scope of the identifier.

    auto& scope_map = try_insert_scope_map(scope);
    auto  it        = scope_map.find(identifier);

    // This should never happen as try_insert should be called just before this. If it
    // does occur we should actually throw some kind of error.
    // TODO(Matthew): Deal with this.
    if (it == scope_map.end()) {
        return { it, false };
    }

    // If the identifier has information already regarding its type, then associating a
    // concrete type should be a failure.
    // TODO(Matthew): This likely means we need one API for associating type during
    //                parsing, and another during type deduction and reflection phases
    //                of the compiler.
    if (!std::holds_alternative<mftype::UnresolvedType>(it->second)) {
        return { it, false };
    }

    it->second = type;

    return { it, true };
}

mfvar::VariableTypeTable::MapEntry
mfvar::VariableTypeTable::associate_type_of_identifier(
    mfvar::Scope         scope,
    mflit::IdentifierIdx identifier,
    mflit::IdentifierIdx type_of_identifier
) {
    // TODO(Matthew): Type association may need to allow for walking up scope tree
    //                to find where identifier was declared. This is only unnecessary
    //                if in every case we have new info to inform type association we
    //                also know the exact scope of the identifier.

    auto& scope_map = try_insert_scope_map(scope);
    auto  it        = scope_map.find(identifier);

    // This should never happen as try_insert should be called just before this. If it
    // does occur we should actually throw some kind of error.
    // TODO(Matthew): Deal with this.
    if (it == scope_map.end()) {
        return { it, false };
    }

    // If the identifier has information already regarding its type, then associating a
    // concrete type should be a failure.
    // TODO(Matthew): This likely means we need one API for associating type during
    //                parsing, and another during type deduction and reflection phases
    //                of the compiler.
    if (!std::holds_alternative<mftype::UnresolvedType>(it->second)) {
        return { it, false };
    }

    it->second = mftype::UnresolvedTypeOf{ .identifier = type_of_identifier };

    return { it, true };
}

mfvar::VariableTypeTable::MapEntry
mfvar::VariableTypeTable::associate_type_held_by_identifier(
    mfvar::Scope         scope,
    mflit::IdentifierIdx identifier,
    mflit::IdentifierIdx type_held_by_identifier
) {
    // TODO(Matthew): Type association may need to allow for walking up scope tree
    //                to find where identifier was declared. This is only unnecessary
    //                if in every case we have new info to inform type association we
    //                also know the exact scope of the identifier.

    auto& scope_map = try_insert_scope_map(scope);
    auto  it        = scope_map.find(identifier);

    // This should never happen as try_insert should be called just before this. If it
    // does occur we should actually throw some kind of error.
    // TODO(Matthew): Deal with this.
    if (it == scope_map.end()) {
        return { it, false };
    }

    // If the identifier has information already regarding its type, then associating a
    // concrete type should be a failure.
    // TODO(Matthew): This likely means we need one API for associating type during
    //                parsing, and another during type deduction and reflection phases
    //                of the compiler.
    if (!std::holds_alternative<mftype::UnresolvedType>(it->second)) {
        return { it, false };
    }

    it->second = mftype::UnresolvedTypeHeldBy{ .identifier = type_held_by_identifier };

    return { it, true };
}

const mftype::Type* mfvar::VariableTypeTable::find(
    mfvar::Scope                scope,
    const mfvar::ScopeTree&     scope_tree,
    const mflit::IdentifierIdx& identifier
) const {
    mfvar::Scope current_scope = scope;

    while (true) {
        const auto* scope_map = get_scope_map(current_scope);
        if (scope_map) {
            auto it = scope_map->find(identifier);
            if (it != scope_map->end()) {
                return &it->second;
            }
        }

        auto in_edges = boost::in_edges(current_scope, scope_tree);
        if (in_edges.first == in_edges.second) {
            // No parent scope
            break;
        }
        current_scope = boost::source(*in_edges.first, scope_tree);
    }

    return nullptr;
}

const mfvar::VariableTypeTable::Map*
mfvar::VariableTypeTable::get_scope_map(mfvar::Scope scope) const {
    auto it = m_scope_var_type_map.find(scope);
    if (it == m_scope_var_type_map.end()) {
        return nullptr;
    }
    return &it->second;
}

mfvar::VariableTypeTable::Map&
mfvar::VariableTypeTable::try_insert_scope_map(mfvar::Scope scope) {
    auto it = m_scope_var_type_map.try_emplace(scope, Map{}).first;
    return it->second;
}
