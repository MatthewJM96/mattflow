#include "stdafx.h"

#include "type/primitive.h"

mftype::PrimitiveType mftype::cast_token_to_intrinsic(mflex::TokenType tok) {
    assert(
        static_cast<int16_t>(tok) > static_cast<int16_t>(PrimitiveType::LOWER_SENTINEL)
    );
    assert(
        static_cast<int16_t>(tok) < static_cast<int16_t>(PrimitiveType::UPPER_SENTINEL)
    );

    return static_cast<PrimitiveType>(static_cast<int16_t>(tok));
}
