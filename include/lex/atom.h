#ifndef __mattflow_atom_h
#define __mattflow_atom_h

namespace mattflow {
    namespace lex {
        using Atom     = std::string;
        using AtomView = std::string_view;
        using AtomIdx  = uint64_t;
        using AtomHash = uint64_t;

        class AtomTable {
        public:
            static AtomTable& get() {
                static AtomTable instance;
                return instance;
            }

            AtomIdx try_insert(std::string_view atom);

            AtomView get(AtomIdx atom_idx);
        protected:
            AtomTable() { }
        public:
            AtomTable(AtomTable const&)      = delete;
            void operator=(AtomTable const&) = delete;
        protected:
            std::unordered_map<AtomHash, AtomIdx> m_hash_idx_map;
            std::vector<Atom>                     m_atoms;
            std::shared_mutex                     m_mutex;
        };
    }  // namespace lex
}  // namespace mattflow
namespace mflex = mattflow::lex;

#endif  // __mattflow_atom_h
