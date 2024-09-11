#include "stdafx.h"

#include "literal/number.h"

mflit::Number::Number() : m_is_floating_point(false), m_int_value(0) {
    // Empty.
}

mflit::Number::Number(std::string_view repr) {
    parse_repr(repr);
}

void mflit::Number::parse_floating_point_repr(std::string_view repr) {
    auto it = repr.begin();

    bool   decimal_point = false;
    size_t min_size      = 0;

    if (repr.size() > 0 && (repr[0] == '-' || repr[0] == '+')) {
        it++;
        min_size++;
    }

    while (it != repr.end()) {
        if (*it == '.') {
            if (!decimal_point) {
                decimal_point = true;
            } else {
                break;
            }
        } else if (!std::isdigit(*it) && ((*it != 'f') || (*it != 'd') || it + 1 != repr.end() || !decimal_point))
        {
            break;
        }
        ++it;
    }

    if (repr.size() <= min_size || it != repr.end()) {
        m_is_bad = true;
        return;
    }

    m_is_floating_point = true;
    m_float_value       = std::atof(repr.data());
}

void mflit::Number::parse_scientific_notation_repr(std::string_view repr) {
    // TODO(Matthew): implement. std::atof etc actually support standard notation, but
    //                atoi doesn't.
    (void)repr;
}

void mflit::Number::parse_integral_repr(std::string_view repr) {
    auto it = repr.begin();

    // Check if the string is empty
    if (repr.empty()) {
        m_is_bad = true;
        return;
    }

    // Handle optional leading sign
    if (*it == '+' || *it == '-') {
        ++it;
    }

    // Ensure there's at least one digit after an optional sign
    if (it == repr.end()) {
        m_is_bad = true;
        return;
    }

    // Check each character to see if it is a digit
    for (; it < repr.end(); ++it) {
        if (!std::isdigit(*it)) {
            m_is_bad = true;
            return;
        }
    }

    m_is_floating_point = false;
    m_int_value         = std::atoi(repr.data());
}

void mflit::Number::parse_repr(std::string_view repr) {
    if (repr.ends_with('d') || repr.ends_with('f')
        || (repr.contains('.') && !repr.contains('d') && !repr.contains('D')
            && !repr.contains('e') && !repr.contains('E')))
    {
        parse_floating_point_repr(repr);
    } else if (repr.contains('d') && repr.contains('D') && repr.contains('e') && repr.contains('E'))
    {
        parse_scientific_notation_repr(repr);
    } else {
        parse_integral_repr(repr);
    }
}
