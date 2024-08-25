#include "stdafx.h"

#include "type/number.h"

mftype::NumberType mftype::cast_token_to_intrinsic(mflex::TokenType tok) {
    assert(static_cast<int16_t>(tok) >= static_cast<int16_t>(mflex::TokenType::CHAR));
    assert(
        static_cast<int16_t>(tok) <= static_cast<int16_t>(mflex::TokenType::FLOAT64)
    );

    return static_cast<NumberType>(static_cast<int16_t>(tok));
}
