#include "stdafx.h"

#include "type.h"

mftype::Type* mftype::TypeTable::register_type_from_token(const mflex::Token& tok) {
    if (tok.type != mflex::TokenType::IDENTIFIER) {
        // TODO(Matthew): implement support for other tokens that can reflect a type.
        assert(false);
    }

    if (m_ident_type_map.contains(tok.identifier_idx)) {
        return m_ident_type_map[tok.identifier_idx];
    }

    m_types.emplace_back(Type{});
    m_ident_type_map[tok.identifier_idx] = &m_types.back();

    return &m_types.back();
}
