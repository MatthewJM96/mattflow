#include "stdafx.h"

#include "number.h"

mf::Number::Number() : m_repr("") {
    // Empty.
}

mf::Number::Number(std::string&& repr) : m_repr(std::move(repr)) {
    // Empty.
}

std::string_view mf::Number::repr() {
    return std::string_view(m_repr);
}
