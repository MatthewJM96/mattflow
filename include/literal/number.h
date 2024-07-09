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

            bool is_floating_point() { return m_is_floating_point; }

            template <typename Type>
                requires (std::floating_point<Type> || std::integral<Type>)
            Type as();
        protected:
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
