#include "stdafx.h"

#include "literal/number.h"

mflit::Number::Number() : m_is_floating_point(false), m_int_value(0) {
    // Empty.
}

mflit::Number::Number(std::string_view repr) {
    // TODO(Matthew): Parse string into number.
    (void)repr;
}
