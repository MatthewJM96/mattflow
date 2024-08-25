#include "stdafx.h"

#include "literal/string.h"

mflit::StringIdx mflit::StringTable::try_insert(std::string&& string) {
    std::unique_lock lck(m_mutex);

    StringHash hash = std::hash<std::string>{}(string);

    if (!m_hash_idx_map.contains(hash)) {
        m_hash_idx_map[hash] = m_strings.size();
        m_strings.emplace_back(std::string(string));
    }

    return m_hash_idx_map[hash];
}

std::string_view mflit::StringTable::get(StringIdx string_idx) {
    std::shared_lock lck(m_mutex);

    if (string_idx < m_strings.size()) {
        return std::string_view(m_strings[string_idx]);
    }
    return std::string_view();
}
