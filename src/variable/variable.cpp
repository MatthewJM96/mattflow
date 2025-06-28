#include "stdafx.h"

#include "type/type.h"

#include "variable/variable.h"

mfvar::VariableTypeTable::MapEntry mfvar::VariableTypeTable::try_insert(
    mfvar::Scope scope, mflit::IdentifierIdx identifier
) {
    auto& scope_map = get_scope_map(scope);
    auto  it        = scope_map.find(identifier);

    if (it == scope_map.end()) {
        return scope_map.insert({ identifier, mftype::UnresolvedType{} });
    }

    return { it, false };
}

mfvar::VariableTypeTable::MapEntry mfvar::VariableTypeTable::try_insert(
    mfvar::Scope scope, mflit::IdentifierIdx identifier, const mftype::Type& type
) {
    auto& scope_map = get_scope_map(scope);
    auto  it        = scope_map.find(identifier);

    if (it == scope_map.end()) {
        return scope_map.insert({ identifier, type });
    }

    return { it, false };
}

mfvar::VariableTypeTable::MapEntry mfvar::VariableTypeTable::associate_type(
    mfvar::Scope scope, mflit::IdentifierIdx identifier, const mftype::Type& type
) {
    auto& scope_map = get_scope_map(scope);
    auto  it        = scope_map.find(identifier);

    if (it == scope_map.end()) {
        return { it, false };
    }

    if (it->second != mftype::Type{ mftype::UnresolvedType{} }) {
        return { it, false };
    }

    it->second = type;

    return { it, true };
}

mfvar::VariableTypeTable::Map&
mfvar::VariableTypeTable::get_scope_map(mfvar::Scope scope) {
    auto it = m_scope_var_type_map.try_emplace(scope, {}).first;
    return it->second;
}
