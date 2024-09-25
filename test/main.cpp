#include "stdafx.h"

#include "lex/lexer.h"
#include "literal/string.h"

#include "ast/debug.h"
#include "ast/parse.h"

using recurse_directory   = std::filesystem::recursive_directory_iterator;
using TokenTypeUnderlying = std::underlying_type_t<mflex::TokenType>;

enum class TestResult {
    UNSPECIFIED_FAILURE,
    LEXING_FAILURE,
    SYNTAX_PARSING_FAILURE,
    SUCCESS
};

static std::tuple<std::filesystem::path, std::filesystem::path>
validation_paths(const std::filesystem::path& path) {
    const auto token_filename = path.filename().replace_extension(".token");
    const auto ast_filename   = path.filename().replace_extension(".dot");

    auto validation_directory = std::filesystem::path("validation");

    bool is_root_part = true;
    for (const auto& part : path.parent_path()) {
        if (is_root_part) {
            is_root_part = false;
            continue;
        }

        validation_directory /= part;
    }

    return { validation_directory / token_filename,
             validation_directory / ast_filename };
}

TestResult
run_test(const std::filesystem::path& path, bool generate_validations = false) {
    mattflow::SourceView source_view;
    if (!mattflow::SourceView::from_filepath(path, source_view)) {
        std::cout << "Could not read test case at path:\n" << path << std::endl;
        exit(1);
    }

    const auto [token_filepath, ast_filepath] = validation_paths(path);

    std::cout << "\nLexing source code..." << std::endl;

    mflex::Tokens tokens;
    mflex::parse(source_view, tokens);

    if (generate_validations) {
        std::cout << "    ...generating token validation file:\n        "
                  << token_filepath << std::endl;

        std::ofstream token_os(token_filepath);
        for (const auto& token : tokens) {
            token_os << static_cast<TokenTypeUnderlying>(token.type) << std::endl;
        }
    } else {
        std::cout << "    ...validating tokens generated against:\n        "
                  << token_filepath << std::endl;

        std::stringstream token_ss;
        for (const auto& token : tokens) {
            token_ss << static_cast<TokenTypeUnderlying>(token.type) << "..."
                     << std::endl;
        }

        std::ifstream     token_is(token_filepath);
        std::stringstream buffer;
        buffer << token_is.rdbuf();

        std::string result   = token_ss.str();
        std::string expected = buffer.str();

        if (result != expected) return TestResult::LEXING_FAILURE;

        std::cout << "validated." << std::endl;
    }

    std::cout << "\nParsing syntax of source code..." << std::endl;

    mfast::AST                  ast;
    mfast::NodeBuffers          node_buffers;
    mftype::IdentifierTypeTable type_table;
    mfast::parse(tokens, ast, node_buffers, type_table);

    if (generate_validations) {
        std::cout << "    ...generating AST validation file:\n        " << ast_filepath
                  << std::endl;

        std::ofstream ast_os(ast_filepath);
        boost::write_graphviz(ast_os, ast, mfast::NodeInfoWriter(&node_buffers));
    } else {
        std::cout << "    ...validating AST generated against:\n        "
                  << ast_filepath << "..." << std::endl;

        std::stringstream ast_ss;
        boost::write_graphviz(ast_ss, ast, mfast::NodeInfoWriter(&node_buffers));

        std::ifstream     ast_is(ast_filepath);
        std::stringstream buffer;
        buffer << ast_is.rdbuf();

        std::string result   = ast_ss.str();
        std::string expected = buffer.str();

        if (result != expected) return TestResult::SYNTAX_PARSING_FAILURE;

        std::cout << "validated." << std::endl;
    }

    std::cout << std::endl;

    return TestResult::SUCCESS;
}

void run_tests(bool generate_validations = false) {
    std::cout << "Running tests...\n" << std::endl;

    size_t successes               = 0;
    size_t lexing_failures         = 0;
    size_t syntax_parsing_failures = 0;
    size_t unspecified_failures    = 0;

    for (const auto& test_case : recurse_directory("samples")) {
        if (!test_case.is_regular_file()) continue;

        std::cout << "\n-------- " << test_case.path() << " --------" << std::endl;
        switch (run_test(test_case.path(), generate_validations)) {
            case TestResult::SUCCESS:
                std::cout << "Result   : SUCCESS" << std::endl;
                successes += 1;
                break;
            case TestResult::LEXING_FAILURE:
                std::cout << "Result   : LEXING_FAILURE" << std::endl;
                lexing_failures += 1;
                break;
            case TestResult::SYNTAX_PARSING_FAILURE:
                std::cout << "Result   : SYNTAX_PARSING_FAILURE" << std::endl;
                syntax_parsing_failures += 1;
                break;
            case TestResult::UNSPECIFIED_FAILURE:
            default:
                std::cout << "Result   : UNSPECIFIED_FAILURE" << std::endl;
                unspecified_failures += 1;
                break;
        }
        std::cout << "\n--------------------";
        for (const auto& _ : test_case.path().string()) std::cout << "-";
        std::cout << std::endl;
    }

    std::cout << "\n----------- SUMMARY -----------\n" << std::endl;
    std::cout << "  Unspecified Failures    : " << unspecified_failures << std::endl;
    std::cout << "  Lexing Failures         : " << lexing_failures << std::endl;
    std::cout << "  Syntax Parsing Failures : " << syntax_parsing_failures << std::endl;
    std::cout << "  Successes               : " << successes << std::endl;
    std::cout << "\n-------------------------------" << std::endl;

    std::cout << "\nTests complete." << std::endl;
}

int main() {
    run_tests();
}
