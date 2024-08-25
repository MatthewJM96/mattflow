#ifndef __mattflow_literal_string_h
#define __mattflow_literal_string_h

namespace mattflow {
    namespace literal {
        using StringIdx  = uint64_t;
        using StringHash = uint64_t;

        class StringTable {
        public:
            MATTFLOW_NON_COPYABLE(StringTable);

            static StringTable& get() {
                static StringTable instance;
                return instance;
            }

            StringIdx try_insert(std::string&& string);

            std::string_view get(StringIdx string_idx);
        protected:
            StringTable() { /* Empty. */
            }
        protected:
            std::unordered_map<StringHash, StringIdx> m_hash_idx_map;
            std::vector<std::string>                  m_strings;
            std::shared_mutex                         m_mutex;
        };
    }  // namespace literal
}  // namespace mattflow
namespace mflit = mattflow::literal;

#endif  // __mattflow_literal_string_h
