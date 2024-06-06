#ifndef __mattflow_token_h
#define __mattflow_token_h

#include "atom.h"

namespace mattflow {
    namespace lex {
        enum class TokenType : int16_t {
            SENTINEL = std::numeric_limits<int16_t>::min(),

            // Parentheses, braces, and brackets.
            LEFT_PAREN,
            RIGHT_PAREN,
            LEFT_BRACE,
            RIGHT_BRACE,
            LEFT_BRACKETS,
            RIGHT_BRACKETS,

            // Separators and accessors.
            COMMA,
            DOT,

            // Assignment.
            LET,

            // Arithmetic operators.
            MINUS,
            PLUS,
            SLASH,
            STAR,

            // Boolean operators.
            NOT,
            EQUALS,
            NOT_EQUALS,
            LESS_THAN,
            LESS_THAN_OR_EQUAL_TO,
            GREATER_THAN,
            GREATER_THAN_OR_EQUAL_TO
        };

        struct Token {
            TokenType type;
            AtomIdx   atom_idx;

            uint64_t line, first_column;
        };
    }  // namespace lex
}  // namespace mattflow
namespace mflex = mattflow::lex;

#endif  // __mattflow_token_h
