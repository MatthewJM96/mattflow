#ifndef __mattflow_lex_token_type_h
#define __mattflow_lex_token_type_h

#if defined(MATTFLOW_OS_MAC)
// MacOS mach/boolean.h sets TRUE and FALSE macros.
#  undef TRUE
#  undef FALSE
#endif  // defined(MATTFLOW_OS_MAC)

namespace mattflow {
    namespace lex {
        enum class TokenType : int16_t {
            SENTINEL = std::numeric_limits<int16_t>::min(),

            // Parentheses, braces, and brackets.
            LEFT_PAREN,     // (
            RIGHT_PAREN,    // )
            LEFT_BRACE,     // {
            RIGHT_BRACE,    // }
            LEFT_BRACKET,   // [
            RIGHT_BRACKET,  // ]

            // Separators and accessors.
            COMMA,     // ,
            SEQUENCE,  // ..
            DOT,       // .

            // Boolean operators.
            NOT,                       // !
            EQUALS,                    // ==
            NOT_EQUALS,                // !=
            LESS_THAN,                 // <
            LESS_THAN_OR_EQUAL_TO,     // <=
            GREATER_THAN,              // >
            GREATER_THAN_OR_EQUAL_TO,  // >=
            OR,                        // or
            AND,                       // and

            // Assignment.
            ASSIGN_DEDUCED_VALUE,  // :=
            DEDUCED_ARROW,         // :->
            ASSIGN_TYPE,           // :
            ASSIGN_VALUE,          // =
            ARROW,                 // ->

            // Arithmetic operators.
            MINUS,  // -
            PLUS,   // +
            SLASH,  // /
            STAR,   // *
            POWER,  // ^

            // Keywords
            IF,      // if
            THEN,    // then
            ELIF,    // elif
            ELSE,    // else
            FOR,     // for
            IN,      // in
            WHERE,   // where
            WHILE,   // while
            DO,      // do
            MATCH,   // match
            PRINT,   // print
            STRUCT,  // struct

            // Boolean values
            TRUE,   // true
            FALSE,  // false

            // Intrinsic types
            NIL,      // null
            CHAR,     // char
            BOOL,     // bool
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

        struct TokenMatcher {
            TokenType   type;
            std::string pattern;
        };

        const std::array<char, 20> KEYWORD_STOPWORDS
            = { ' ', '\n', '(', ')', '{', '}', '[', ']', ',', '.',
                '-', '!',  '=', '<', '>', ':', '+', '/', '*', '^' };

        static std::array<TokenMatcher, 26> EXACT_TOKEN_MATCHERS = {
            TokenMatcher{              TokenType::LEFT_PAREN,   "("},
            TokenMatcher{             TokenType::RIGHT_PAREN,   ")"},
            TokenMatcher{              TokenType::LEFT_BRACE,   "{"},
            TokenMatcher{             TokenType::RIGHT_BRACE,   "}"},
            TokenMatcher{            TokenType::LEFT_BRACKET,   "["},
            TokenMatcher{           TokenType::RIGHT_BRACKET,   "]"},
            TokenMatcher{                   TokenType::COMMA,   ","},
            TokenMatcher{                TokenType::SEQUENCE,  ".."},
            TokenMatcher{                     TokenType::DOT,   "."},
            TokenMatcher{              TokenType::NOT_EQUALS,  "!="},
            TokenMatcher{                     TokenType::NOT,   "!"},
            TokenMatcher{                  TokenType::EQUALS,  "=="},
            TokenMatcher{               TokenType::LESS_THAN,   "<"},
            TokenMatcher{   TokenType::LESS_THAN_OR_EQUAL_TO,  "<="},
            TokenMatcher{            TokenType::GREATER_THAN,   ">"},
            TokenMatcher{TokenType::GREATER_THAN_OR_EQUAL_TO,  ">="},
            TokenMatcher{    TokenType::ASSIGN_DEDUCED_VALUE,  ":="},
            TokenMatcher{           TokenType::DEDUCED_ARROW, ":->"},
            TokenMatcher{             TokenType::ASSIGN_TYPE,   ":"},
            TokenMatcher{            TokenType::ASSIGN_VALUE,   "="},
            TokenMatcher{                   TokenType::ARROW,  "->"},
            TokenMatcher{                   TokenType::MINUS,   "-"},
            TokenMatcher{                    TokenType::PLUS,   "+"},
            TokenMatcher{                   TokenType::SLASH,   "/"},
            TokenMatcher{                    TokenType::STAR,   "*"},
            TokenMatcher{                   TokenType::POWER,   "^"}
        };

        static std::array<TokenMatcher, 31> KEYWORD_TOKEN_MATCHERS = {
            TokenMatcher{     TokenType::OR,      "or"},
            TokenMatcher{    TokenType::AND,     "and"},
            TokenMatcher{     TokenType::IF,      "if"},
            TokenMatcher{   TokenType::THEN,    "then"},
            TokenMatcher{   TokenType::ELIF,    "elif"},
            TokenMatcher{   TokenType::ELSE,    "else"},
            TokenMatcher{    TokenType::FOR,     "for"},
            TokenMatcher{     TokenType::IN,      "in"},
            TokenMatcher{  TokenType::WHERE,   "where"},
            TokenMatcher{  TokenType::WHILE,   "while"},
            TokenMatcher{     TokenType::DO,      "do"},
            TokenMatcher{  TokenType::MATCH,   "match"},
            TokenMatcher{  TokenType::PRINT,   "print"},
            TokenMatcher{ TokenType::STRUCT,  "struct"},
            TokenMatcher{   TokenType::TRUE,    "true"},
            TokenMatcher{  TokenType::FALSE,   "false"},
            TokenMatcher{    TokenType::NIL,    "null"},
            TokenMatcher{   TokenType::CHAR,    "char"},
            TokenMatcher{   TokenType::BOOL,    "bool"},
            TokenMatcher{    TokenType::INT,     "int"},
            TokenMatcher{   TokenType::INT8,    "int8"},
            TokenMatcher{  TokenType::INT16,   "int16"},
            TokenMatcher{  TokenType::INT32,   "int32"},
            TokenMatcher{  TokenType::INT64,   "int64"},
            TokenMatcher{   TokenType::UINT,    "uint"},
            TokenMatcher{  TokenType::UINT8,   "uint8"},
            TokenMatcher{ TokenType::UINT16,  "uint16"},
            TokenMatcher{ TokenType::UINT32,  "uint32"},
            TokenMatcher{ TokenType::UINT64,  "uint64"},
            TokenMatcher{TokenType::FLOAT32, "float32"},
            TokenMatcher{TokenType::FLOAT64, "float64"},
        };
    }  // namespace lex
}  // namespace mattflow
namespace mflex = mattflow::lex;

#endif  // __mattflow_lex_token_type_h
