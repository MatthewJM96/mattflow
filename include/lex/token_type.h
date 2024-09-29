#ifndef __mattflow_lex_token_type_h
#define __mattflow_lex_token_type_h

#if defined(MATTFLOW_OS_MAC)
// MacOS mach/boolean.h sets TRUE and FALSE macros.
#  undef TRUE
#  undef FALSE
#endif  // defined(MATTFLOW_OS_MAC)

namespace mattflow {
    namespace lex {
        enum class TokenMatchingStrategy {
            EXACT,
            KEYWORD,
            IDENTIFIER,
            STRING,
            NUMBER
        };

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
            COMMA,  // ,
            RANGE,  // ..
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
            OR,                        // or
            AND,                       // and

            // Assignment.
            ASSIGN_TYPE,   // :
            ASSIGN_VALUE,  // =

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
            TokenType             type;
            TokenMatchingStrategy matching_strategy;
            std::string           pattern;
        };

        const std::vector<char> KEYWORD_STOPWORDS
            = { ' ', '\n', '(', ')', '{', '}', '[', ']', ',', '.',
                '-', '!',  '=', '<', '>', ':', '+', '/', '*', '^' };

        static std::vector<TokenMatcher> TOKEN_MATCHERS = {
            {              TokenType::LEFT_PAREN,TokenMatchingStrategy::EXACT,            "("                                                                                 },
            {             TokenType::RIGHT_PAREN,   TokenMatchingStrategy::EXACT,                                            ")"},
            {              TokenType::LEFT_BRACE,   TokenMatchingStrategy::EXACT,                                            "{"},
            {             TokenType::RIGHT_BRACE,   TokenMatchingStrategy::EXACT,                                            "}"},
            {            TokenType::LEFT_BRACKET,   TokenMatchingStrategy::EXACT,                                            "["},
            {           TokenType::RIGHT_BRACKET,   TokenMatchingStrategy::EXACT,                                            "]"},
            {                   TokenType::COMMA,   TokenMatchingStrategy::EXACT,                                            ","},
            {                   TokenType::RANGE,   TokenMatchingStrategy::EXACT,                                           ".."},
            {                     TokenType::DOT,   TokenMatchingStrategy::EXACT,                                            "."},
            {                   TokenType::ARROW,   TokenMatchingStrategy::EXACT,                                           "->"},
            {              TokenType::NOT_EQUALS,   TokenMatchingStrategy::EXACT,                                           "!="},
            {                     TokenType::NOT,   TokenMatchingStrategy::EXACT,                                            "!"},
            {                  TokenType::EQUALS,   TokenMatchingStrategy::EXACT,                                           "=="},
            {               TokenType::LESS_THAN,   TokenMatchingStrategy::EXACT,                                            "<"},
            {   TokenType::LESS_THAN_OR_EQUAL_TO,   TokenMatchingStrategy::EXACT,                                           "<="},
            {            TokenType::GREATER_THAN,   TokenMatchingStrategy::EXACT,                                            ">"},
            {TokenType::GREATER_THAN_OR_EQUAL_TO,   TokenMatchingStrategy::EXACT,                                           ">="},
            {             TokenType::ASSIGN_TYPE,   TokenMatchingStrategy::EXACT,                                            ":"},
            {            TokenType::ASSIGN_VALUE,   TokenMatchingStrategy::EXACT,                                            "="},
            {                   TokenType::MINUS,   TokenMatchingStrategy::EXACT,                                            "-"},
            {                    TokenType::PLUS,   TokenMatchingStrategy::EXACT,                                            "+"},
            {                   TokenType::SLASH,   TokenMatchingStrategy::EXACT,                                            "/"},
            {                    TokenType::STAR,   TokenMatchingStrategy::EXACT,                                            "*"},
            {                   TokenType::POWER,   TokenMatchingStrategy::EXACT,                                            "^"},
            {                      TokenType::OR, TokenMatchingStrategy::KEYWORD,                                           "or"},
            {                     TokenType::AND, TokenMatchingStrategy::KEYWORD,                                          "and"},
            {                      TokenType::IF, TokenMatchingStrategy::KEYWORD,                                           "if"},
            {                    TokenType::THEN, TokenMatchingStrategy::KEYWORD,                                         "then"},
            {                    TokenType::ELIF, TokenMatchingStrategy::KEYWORD,                                         "elif"},
            {                    TokenType::ELSE, TokenMatchingStrategy::KEYWORD,                                         "else"},
            {                     TokenType::FOR, TokenMatchingStrategy::KEYWORD,                                          "for"},
            {                      TokenType::IN, TokenMatchingStrategy::KEYWORD,                                           "in"},
            {                   TokenType::WHERE, TokenMatchingStrategy::KEYWORD,                                        "where"},
            {                   TokenType::WHILE, TokenMatchingStrategy::KEYWORD,                                        "while"},
            {                      TokenType::DO, TokenMatchingStrategy::KEYWORD,                                           "do"},
            {                   TokenType::MATCH, TokenMatchingStrategy::KEYWORD,                                        "match"},
            {                   TokenType::PRINT, TokenMatchingStrategy::KEYWORD,                                        "print"},
            {                  TokenType::STRUCT, TokenMatchingStrategy::KEYWORD,                                       "struct"},
            {                    TokenType::TRUE, TokenMatchingStrategy::KEYWORD,                                         "true"},
            {                   TokenType::FALSE, TokenMatchingStrategy::KEYWORD,                                        "false"},
            {                     TokenType::NIL, TokenMatchingStrategy::KEYWORD,                                         "null"},
            {                    TokenType::CHAR, TokenMatchingStrategy::KEYWORD,                                         "char"},
            {                    TokenType::BOOL, TokenMatchingStrategy::KEYWORD,                                         "bool"},
            {                     TokenType::INT, TokenMatchingStrategy::KEYWORD,                                          "int"},
            {                    TokenType::INT8, TokenMatchingStrategy::KEYWORD,                                         "int8"},
            {                   TokenType::INT16, TokenMatchingStrategy::KEYWORD,                                        "int16"},
            {                   TokenType::INT32, TokenMatchingStrategy::KEYWORD,                                        "int32"},
            {                   TokenType::INT64, TokenMatchingStrategy::KEYWORD,                                        "int64"},
            {                    TokenType::UINT, TokenMatchingStrategy::KEYWORD,                                         "uint"},
            {                   TokenType::UINT8, TokenMatchingStrategy::KEYWORD,                                        "uint8"},
            {                  TokenType::UINT16, TokenMatchingStrategy::KEYWORD,                                       "uint16"},
            {                  TokenType::UINT32, TokenMatchingStrategy::KEYWORD,                                       "uint32"},
            {                  TokenType::UINT64, TokenMatchingStrategy::KEYWORD,                                       "uint64"},
            {                 TokenType::FLOAT32, TokenMatchingStrategy::KEYWORD,                                      "float32"},
            {                 TokenType::FLOAT64, TokenMatchingStrategy::KEYWORD,                                      "float64"},
            {              TokenType::IDENTIFIER,
             TokenMatchingStrategy::IDENTIFIER,
             "^(?:[a-zA-Z_][a-zA-Z_0-9]*)"                                                                                      },
            {                  TokenType::STRING,
             TokenMatchingStrategy::STRING,
             "^(?:(?:\\\"{3}((?:[^\\\"]*)*)\\\"{3})|(?:\\\"([^\\\"\\n]*)\\\"))"                                                 },
            {                  TokenType::NUMBER,
             TokenMatchingStrategy::NUMBER,
             "^(?:[0-9]+(?:\\.[0-9]*)?)"                                                                                        },
        };
    }  // namespace lex
}  // namespace mattflow
namespace mflex = mattflow::lex;

#endif  // __mattflow_lex_token_type_h
