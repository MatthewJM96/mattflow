#include "stdafx.h"

#include "literal/identifier.h"

mflit::IdentifierIdx mflit::IdentifierTable::try_insert(std::string_view identifier) {
    std::unique_lock lck(m_mutex);

    IdentifierHash hash = std::hash<std::string_view>{}(identifier);

    if (!m_hash_idx_map.contains(hash)) {
        m_hash_idx_map[hash] = m_identifiers.size();
        m_identifiers.emplace_back(std::string(identifier));
    }

    return m_hash_idx_map[hash];
}

std::string_view mflit::IdentifierTable::get(IdentifierIdx identifier_idx) {
    std::shared_lock lck(m_mutex);

    if (identifier_idx < m_identifiers.size()) {
        return std::string_view(m_identifiers[identifier_idx]);
    }
    return std::string_view();
}
