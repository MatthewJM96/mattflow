#include "stdafx.h"

#include "literal/number.h"

mflit::Number::Number() : m_is_floating_point(false), m_int_value(0) {
    // Empty.
}

mflit::Number::Number(std::string_view repr) {
    // TODO(Matthew): Parse string into number.
    (void)repr;
}

template <typename Type>
    requires (std::floating_point<Type> || std::integral<Type>)
Type mflit::Number::as() {
    if constexpr (std::floating_point<Type>) {
        assert(m_is_floating_point);
    } else {
        assert(!m_is_floating_point);
    }

    return *reinterpret_cast<Type*>(reinterpret_cast<void*>(&m_int_value));
}
