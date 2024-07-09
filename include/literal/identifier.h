#ifndef __mattflow_literal_identifier_h
#define __mattflow_literal_identifier_h

namespace mattflow {
    namespace literal {
        using IdentifierIdx  = uint64_t;
        using IdentifierHash = uint64_t;

        class IdentifierTable {
        public:
            MATTFLOW_NON_COPYABLE(IdentifierTable);

            static IdentifierTable& get() {
                static IdentifierTable instance;
                return instance;
            }

            IdentifierIdx try_insert(std::string_view identifier);

            std::string_view get(IdentifierIdx identifier_idx);
        protected:
            IdentifierTable() { /* Empty. */
            }
        protected:
            std::unordered_map<IdentifierHash, IdentifierIdx> m_hash_idx_map;
            std::vector<std::string>                          m_identifiers;
            std::shared_mutex                                 m_mutex;
        };
    }  // namespace literal
}  // namespace mattflow
namespace mflit = mattflow::literal;

#endif  // __mattflow_literal_identifier_h
