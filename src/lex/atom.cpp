#include "stdafx.h"

#include "lex/atom.h"

mflex::AtomIdx mflex::AtomTable::try_insert(std::string_view atom) {
    std::unique_lock lck(m_mutex);

    AtomHash hash = std::hash<std::string_view>{}(atom);

    if (!m_hash_idx_map.contains(hash)) {
        m_hash_idx_map[hash] = m_atoms.size();
        m_atoms.emplace_back(Atom(atom));
    }

    return m_hash_idx_map[hash];
}

mflex::AtomView mflex::AtomTable::get(AtomIdx atom_idx) {
    std::shared_lock lck(m_mutex);

    if (atom_idx < m_atoms.size()) {
        return AtomView(m_atoms[atom_idx]);
    }
    return AtomView();
}
