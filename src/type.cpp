#include "stdafx.h"

#include "type.h"

mftype::Type* mftype::TypeTable::register_type(mflit::IdentifierIdx identifier) {
    if (m_ident_type_map.contains(identifier)) {
        return m_ident_type_map[identifier];
    }

    m_types.emplace_back(Type{});
    m_ident_type_map[identifier] = &m_types.back();

    return &m_types.back();
}
