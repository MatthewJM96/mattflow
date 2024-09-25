#include "stdafx.h"

#include "type/primitive.h"

mftype::PrimitiveType mftype::cast_token_to_intrinsic(mflex::TokenType tok) {
    mfassert(
        static_cast<int16_t>(tok) > static_cast<int16_t>(PrimitiveType::LOWER_SENTINEL),
        "Tried to cast a token to intrinsic that is not an intrinsic."
    );
    mfassert(
        static_cast<int16_t>(tok) < static_cast<int16_t>(PrimitiveType::UPPER_SENTINEL),
        "Tried to cast a token to intrinsic that is not an intrinsic."
    );

    return static_cast<PrimitiveType>(static_cast<int16_t>(tok));
}
