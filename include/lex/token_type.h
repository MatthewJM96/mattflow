#ifndef __mattflow_lex_token_type_h
#define __mattflow_lex_token_type_h

namespace mattflow {
    namespace lex {
        enum class TokenType : int16_t {
            SENTINEL = std::numeric_limits<int16_t>::min(),

            // Parentheses, braces, and brackets.
            LEFT_PAREN,      // (
            RIGHT_PAREN,     // )
            LEFT_BRACE,      // {
            RIGHT_BRACE,     // }
            LEFT_BRACKETS,   // [
            RIGHT_BRACKETS,  // ]

            // Separators and accessors.
            COMMA,  // ,
            DOT,    // .
            ARROW,  // ->

            // Boolean operators.
            NOT,                       // !
            EQUALS,                    // ==
            NOT_EQUALS,                // !=
            LESS_THAN,                 // <
            LESS_THAN_OR_EQUAL_TO,     // <=
            GREATER_THAN,              // >
            GREATER_THAN_OR_EQUAL_TO,  // >=

            // Assignment.
            ASSIGN_TYPE,   // :
            ASSIGN_VALUE,  // =

            // Arithmetic operators.
            MINUS,  // -
            PLUS,   // +
            SLASH,  // /
            STAR,   // *

            // Keywords
            TRUE,    // true
            FALSE,   // false
            OR,      // or
            AND,     // and
            IF,      // if
            THEN,    // then
            ELSE,    // else
            FOR,     // for
            IN,      // in
            WHERE,   // where
            WHILE,   // while
            DO,      // do
            MATCH,   // match
            PRINT,   // print
            STRUCT,  // struct

            // Intrinsic types
            NIL,      // null
            CHAR,     // char
            INT,      // int
            INT8,     // int8
            INT16,    // int16
            INT32,    // int32
            INT64,    // int64
            UINT,     // uint
            UINT8,    // uint8
            UINT16,   // uint16
            UINT32,   // uint32
            UINT64,   // uint64
            FLOAT32,  // float32
            FLOAT64,  // float64

            // Literals
            IDENTIFIER,  // [a-zA-Z_][a-zA-Z0-9_]*
            STRING,      // (?:\"{3}((?:\n.*)*)\"{3})|(?:\"(.*)\")
            // TODO(Matthew): support other number representations?
            NUMBER,  // [0-9]+(?:\.[0-9]*)?
        };

        static std::vector<std::tuple<TokenType, std::string>> TOKEN_REGEX_PATTERNS = {
            {              TokenType::LEFT_PAREN,                                             "\\("},
            {             TokenType::RIGHT_PAREN,                                             "\\)"},
            {              TokenType::LEFT_BRACE,                                             "\\{"},
            {             TokenType::RIGHT_BRACE,                                             "\\}"},
            {           TokenType::LEFT_BRACKETS,                                             "\\["},
            {          TokenType::RIGHT_BRACKETS,                                             "\\]"},
            {                   TokenType::COMMA,                                               ","},
            {                     TokenType::DOT,                                             "\\."},
            {                   TokenType::ARROW,                                              "->"},
            {                     TokenType::NOT,                                               "!"},
            {                  TokenType::EQUALS,                                              "=="},
            {              TokenType::NOT_EQUALS,                                              "!="},
            {               TokenType::LESS_THAN,                                               "<"},
            {   TokenType::LESS_THAN_OR_EQUAL_TO,                                              "<="},
            {            TokenType::GREATER_THAN,                                               ">"},
            {TokenType::GREATER_THAN_OR_EQUAL_TO,                                              ">="},
            {             TokenType::ASSIGN_TYPE,                                               ":"},
            {            TokenType::ASSIGN_VALUE,                                               "="},
            {                   TokenType::MINUS,                                               "-"},
            {                    TokenType::PLUS,                                             "\\+"},
            {                   TokenType::SLASH,                                               "/"},
            {                    TokenType::STAR,                                             "\\*"},
            {                    TokenType::TRUE,                            "true(?![a-zA-Z0-9_])"},
            {                   TokenType::FALSE,                           "false(?![a-zA-Z0-9_])"},
            {                      TokenType::OR,                              "or(?![a-zA-Z0-9_])"},
            {                     TokenType::AND,                             "and(?![a-zA-Z0-9_])"},
            {                      TokenType::IF,                              "if(?![a-zA-Z0-9_])"},
            {                    TokenType::THEN,                            "then(?![a-zA-Z0-9_])"},
            {                    TokenType::ELSE,                            "else(?![a-zA-Z0-9_])"},
            {                     TokenType::FOR,                             "for(?![a-zA-Z0-9_])"},
            {                      TokenType::IN,                              "in(?![a-zA-Z0-9_])"},
            {                   TokenType::WHERE,                           "where(?![a-zA-Z0-9_])"},
            {                   TokenType::WHILE,                           "while(?![a-zA-Z0-9_])"},
            {                      TokenType::DO,                              "do(?![a-zA-Z0-9_])"},
            {                   TokenType::MATCH,                           "match(?![a-zA-Z0-9_])"},
            {                   TokenType::PRINT,                           "print(?![a-zA-Z0-9_])"},
            {                  TokenType::STRUCT,                          "struct(?![a-zA-Z0-9_])"},
            {                     TokenType::NIL,                            "null(?![a-zA-Z0-9_])"},
            {                    TokenType::CHAR,                            "char(?![a-zA-Z0-9_])"},
            {                     TokenType::INT,                             "int(?![a-zA-Z0-9_])"},
            {                    TokenType::INT8,                            "int8(?![a-zA-Z0-9_])"},
            {                   TokenType::INT16,                           "int16(?![a-zA-Z0-9_])"},
            {                   TokenType::INT32,                           "int32(?![a-zA-Z0-9_])"},
            {                   TokenType::INT64,                           "int64(?![a-zA-Z0-9_])"},
            {                    TokenType::UINT,                            "uint(?![a-zA-Z0-9_])"},
            {                   TokenType::UINT8,                           "uint8(?![a-zA-Z0-9_])"},
            {                  TokenType::UINT16,                          "uint16(?![a-zA-Z0-9_])"},
            {                  TokenType::UINT32,                          "uint32(?![a-zA-Z0-9_])"},
            {                  TokenType::UINT64,                          "uint64(?![a-zA-Z0-9_])"},
            {                 TokenType::FLOAT32,                         "float32(?![a-zA-Z0-9_])"},
            {                 TokenType::FLOAT64,                         "float64(?![a-zA-Z0-9_])"},
            {              TokenType::IDENTIFIER,                          "[a-zA-Z_][a-zA-Z_0-9]*"},
            {                  TokenType::STRING, "(?:\\\"{3}((?:\\n.*)*)\\\"{3})|(?:\\\"(.*)\\\")"},
            {                  TokenType::NUMBER,                            "[0-9]+(?:\\.[0-9]*)?"},
        };
    }  // namespace lex
}  // namespace mattflow
namespace mflex = mattflow::lex;

#endif  // __mattflow_lex_token_type_h
