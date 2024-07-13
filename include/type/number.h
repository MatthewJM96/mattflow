#ifndef __mattflow_type_number_h
#define __mattflow_type_number_h

#include "lex/token_type.h"

namespace mattflow {
    namespace type {
        enum class NumberType : int16_t {
            CHAR    = static_cast<int16_t>(mflex::TokenType::CHAR),
            INT     = static_cast<int16_t>(mflex::TokenType::INT),
            INT8    = static_cast<int16_t>(mflex::TokenType::INT8),
            INT16   = static_cast<int16_t>(mflex::TokenType::INT16),
            INT32   = static_cast<int16_t>(mflex::TokenType::INT32),
            INT64   = static_cast<int16_t>(mflex::TokenType::INT64),
            UINT    = static_cast<int16_t>(mflex::TokenType::UINT),
            UINT8   = static_cast<int16_t>(mflex::TokenType::UINT8),
            UINT16  = static_cast<int16_t>(mflex::TokenType::UINT16),
            UINT32  = static_cast<int16_t>(mflex::TokenType::UINT32),
            UINT64  = static_cast<int16_t>(mflex::TokenType::UINT64),
            FLOAT32 = static_cast<int16_t>(mflex::TokenType::FLOAT32),
            FLOAT64 = static_cast<int16_t>(mflex::TokenType::FLOAT64),
        };

        NumberType cast_token_to_intrinsic(mflex::TokenType tok) {
            assert(
                static_cast<int16_t>(tok)
                >= static_cast<int16_t>(mflex::TokenType::CHAR)
            );
            assert(
                static_cast<int16_t>(tok)
                <= static_cast<int16_t>(mflex::TokenType::FLOAT64)
            );

            return static_cast<NumberType>(static_cast<int16_t>(tok));
        }
    }  // namespace type
}  // namespace mattflow
namespace mftype = mattflow::type;

#endif  // __mattflow_type_number_h
