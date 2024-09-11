#ifndef __mattflow_literal_number_h
#define __mattflow_literal_number_h

namespace mattflow {
    namespace literal {
        // NOTE(Matthew): Might need some way of reporting bad number, right now the
        //                permissible numbers via regex don't allow for bad numbers, but
        //                conceivably could in the future. We for now will report during
        //                lexing on any ill-formed literals that might be numbers.
        class Number {
        public:
            Number();
            Number(std::string_view repr);

            bool is_bad() { return m_is_bad; }

            bool is_floating_point() { return m_is_floating_point; }

            template <typename Type>
                requires (std::floating_point<Type> || std::integral<Type>)
            Type as() {
                if constexpr (std::floating_point<Type>) {
                    assert(m_is_floating_point);
                } else {
                    assert(!m_is_floating_point);
                }

                return *reinterpret_cast<Type*>(reinterpret_cast<void*>(&m_int_value));
            }
        protected:
        protected:
            void parse_floating_point_repr(std::string_view repr);
            void parse_scientific_notation_repr(std::string_view repr);
            void parse_integral_repr(std::string_view repr);
            void parse_repr(std::string_view repr);

            bool m_is_bad;
            bool m_is_floating_point;

            union {
                double   m_float_value;
                uint64_t m_int_value;
            };
        };
    }  // namespace literal
}  // namespace mattflow
namespace mflit = mattflow::literal;

#endif  // __mattflow_literal_number_h
