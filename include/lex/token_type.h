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

            // Literals
            IDENTIFIER,  // [a-zA-Z_][a-zA-Z0-9_]*
            STRING,      // (?:\"{3}((?:\n.*)*)\"{3})|(?:\"(.*)\")
            // TODO(Matthew): support other number representations?
            NUMBER,  // [0-9]+(?:\.[0-9]*)?

            // Keywords
            TRUE,   // true
            FALSE,  // false
            OR,     // or
            AND,    // and
            IF,     // if
            THEN,   // then
            ELSE,   // else
            FOR,    // for
            IN,     // in
            MATCH,  // match
            PRINT,  // print
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
            {                     TokenType::NOT,                                               "!"},
            {                  TokenType::EQUALS,                                              "=="},
            {              TokenType::NOT_EQUALS,                                              "!="},
            {               TokenType::LESS_THAN,                                               "<"},
            {   TokenType::LESS_THAN_OR_EQUAL_TO,                                              "<="},
            {            TokenType::GREATER_THAN,                                               ">"},
            {TokenType::GREATER_THAN_OR_EQUAL_TO,                                              ">="},
            {             TokenType::ASSIGN_TYPE,                                               ":"},
            {                   TokenType::MINUS,                                               "-"},
            {                    TokenType::PLUS,                                             "\\+"},
            {                   TokenType::SLASH,                                               "/"},
            {                    TokenType::STAR,                                             "\\*"},
            {              TokenType::IDENTIFIER,                          "[a-zA-Z_][a-zA-Z_0-9]*"},
            {                  TokenType::STRING, "(?:\\\"{3}((?:\\n.*)*)\\\"{3})|(?:\\\"(.*)\\\")"},
            {                  TokenType::NUMBER,                            "[0-9]+(?:\\.[0-9]*)?"},
            {                    TokenType::TRUE,                                            "true"},
            {                   TokenType::FALSE,                                           "false"},
            {                      TokenType::OR,                                              "or"},
            {                     TokenType::AND,                                             "and"},
            {                      TokenType::IF,                                              "if"},
            {                    TokenType::THEN,                                            "then"},
            {                    TokenType::ELSE,                                            "else"},
            {                     TokenType::FOR,                                             "for"},
            {                      TokenType::IN,                                              "in"},
            {                   TokenType::MATCH,                                           "match"},
            {                   TokenType::PRINT,                                           "print"},
        };
    }  // namespace lex
}  // namespace mattflow
namespace mflex = mattflow::lex;

#endif  // __mattflow_lex_token_type_h
