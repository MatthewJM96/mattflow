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
    mflit::IdentifierIdx identifier, const mflex::Token& token
) {
    auto it = m_ident_type_map.find(identifier);

    if (it->second != Type{ UnresolvedType{} }) {
        return { false, it };
    }

    /**
     * We only expect simple types to be assocaited via their token, structs and so on
     * should be associated through other function calls probably - maybe a dedicated
     * multi-function API each for building up a function or struct type?
     */
    if (token.type == mflex::TokenType::IDENTIFIER) {
        // TODO: Do we want this to be handled here for TypeType stuff? I think so but
        // probably with some checks.
        it->second = m_ident_type_map[token.identifier_idx];
    } else if (static_cast<int16_t>(token.type) > static_cast<int16_t>(PrimitiveType::LOWER_SENTINEL) && static_cast<int16_t>(token.type) < static_cast<int16_t>(PrimitiveType::UPPER_SENTINEL))
    {
        it->second = cast_token_to_intrinsic(token.type);
    } else {
        debug_printf("Attempted to associate a non-type as the type of an identifier");
        assert(false);
    }

    return { true, it };
}
