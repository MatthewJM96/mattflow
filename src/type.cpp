#include "stdafx.h"

#include "type.h"

mftype::IdentifierTypeTable::MapEntry
mftype::IdentifierTypeTable::register_identifier(mflit::IdentifierIdx identifier) {
    if (m_ident_type_map.contains(identifier)) {
        return m_ident_type_map.find(identifier);
    }

    m_ident_type_map[identifier] = UnresolvedType{};

    return m_ident_type_map.find(identifier);
}

std::tuple<bool, mftype::IdentifierTypeTable::MapEntry>
mftype::IdentifierTypeTable::associate_type(
    mflit::IdentifierIdx identifier, const mftype::Type& type
) {
    auto it = m_ident_type_map.find(identifier);

    if (it->second != Type{ UnresolvedType{} }) {
        return { false, it };
    }

    it->second = type;

    return { true, it };
}
