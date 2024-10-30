#include "stdafx.h"

#include "type/type.h"

#include "variable/variable.h"

mfvar::VariableTypeTable::MapEntry
mfvar::VariableTypeTable::try_insert(mflit::IdentifierIdx identifier) {
    auto it = m_var_type_map.find(identifier);

    if (it == m_var_type_map.end()) {
        return m_var_type_map.insert({ identifier, mftype::UnresolvedType{} });
    }

    return { it, false };
}

mfvar::VariableTypeTable::MapEntry mfvar::VariableTypeTable::try_insert(
    mflit::IdentifierIdx identifier, const mftype::Type& type
) {
    auto it = m_var_type_map.find(identifier);

    if (it == m_var_type_map.end()) {
        return m_var_type_map.insert({ identifier, type });
    }

    return { it, false };
}

mfvar::VariableTypeTable::MapEntry mfvar::VariableTypeTable::associate_type(
    mflit::IdentifierIdx identifier, const mftype::Type& type
) {
    auto it = m_var_type_map.find(identifier);

    if (it == m_var_type_map.end()) {
        return { it, false };
    }

    if (it->second != mftype::Type{ mftype::UnresolvedType{} }) {
        return { it, false };
    }

    it->second = type;

    return { it, true };
}
