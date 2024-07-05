#include "stdafx.h"

#include "lex/lexer.h"

int main() {
    std::cout << "Hello, world!" << std::endl;

    std::string source
        = "  \
and or then if = *          \
    ";

    mf::SourceView source_view;
    source_view.source       = std::string_view(source);
    source_view.file_id      = 0;
    source_view.start_line   = 0;
    source_view.start_column = 0;
    source_view.end_line     = 2;
    source_view.end_column   = 3;

    mflex::Tokens tokens;
    mflex::parse(source_view, tokens);

    for (auto token : tokens) {
        std::cout << static_cast<int16_t>(token.type) << std::endl;
    }

    std::cout << "and  " << static_cast<int16_t>(mflex::TokenType::AND) << std::endl;
    std::cout << "or   " << static_cast<int16_t>(mflex::TokenType::OR) << std::endl;
    std::cout << "then " << static_cast<int16_t>(mflex::TokenType::THEN) << std::endl;
    std::cout << "if   " << static_cast<int16_t>(mflex::TokenType::IF) << std::endl;
    std::cout << "star " << static_cast<int16_t>(mflex::TokenType::STAR) << std::endl;
}
