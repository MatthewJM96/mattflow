#include "stdafx.h"

#include "lex/lexer.h"
#include "literal/string.h"

#include "ast/debug.h"
#include "ast/parse.h"

using recurse_directory   = std::filesystem::recursive_directory_iterator;
using TokenTypeUnderlying = std::underlying_type_t<mflex::TokenType>;

struct TestConfig {
    bool write_results        = false;
    bool plot_ast_graphs      = false;
    bool generate_validations = false;
};

enum class TestResult {
    UNSPECIFIED_FAILURE,
    LEXING_FAILURE,
    SYNTAX_PARSING_FAILURE,
    SUCCESS
};

static std::tuple<std::filesystem::path, std::filesystem::path>
make_paths(const std::filesystem::path& path, std::filesystem::path&& new_base) {
    const auto token_filename = path.filename().replace_extension(".token");
    const auto ast_filename   = path.filename().replace_extension(".dot");

    auto validation_directory = new_base;

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

static std::tuple<std::filesystem::path, std::filesystem::path>
make_validation_paths(const std::filesystem::path& path) {
    return make_paths(path, "validation");
}

static std::tuple<std::filesystem::path, std::filesystem::path>
make_result_paths(const std::filesystem::path& path) {
    auto result_paths = make_paths(path, "results");

    // Ensure the results directory exists.
    std::filesystem::create_directories(std::get<0>(result_paths).parent_path());

    return result_paths;
}

TestResult run_test(const std::filesystem::path& path, TestConfig config = {}) {
    mattflow::SourceView source_view;
    if (!mattflow::SourceView::from_filepath(path, source_view)) {
        std::cout << "Could not read test case at path:\n" << path << std::endl;
        exit(1);
    }

    const auto [token_validation_filepath, ast_validation_filepath]
        = make_validation_paths(path);
    const auto [token_result_filepath, ast_result_filepath] = make_result_paths(path);

    std::cout << "\nLexing source code..." << std::endl;

    mflex::Tokens tokens;
    mflex::parse(source_view, tokens);

    if (config.generate_validations) {
        std::cout << "    ...generating token validation file:\n        "
                  << token_validation_filepath << "..." << std::endl;

        std::ofstream token_os(token_validation_filepath);
        for (const auto& token : tokens) {
            token_os << static_cast<TokenTypeUnderlying>(token.type) << std::endl;
        }

        std::cout << "generated." << std::endl;
    } else {
        std::cout << "    ...validating tokens generated against:\n        "
                  << token_validation_filepath << "..." << std::endl;

        std::stringstream token_ss;
        for (const auto& token : tokens) {
            token_ss << static_cast<TokenTypeUnderlying>(token.type) << std::endl;
        }

        if (config.write_results) {
            std::ofstream token_os(token_result_filepath);
            token_os << token_ss.str();
        }

        std::ifstream     token_is(token_validation_filepath);
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

    if (config.generate_validations) {
        std::cout << "    ...generating AST validation file:\n        "
                  << ast_validation_filepath << "..." << std::endl;

        std::ofstream ast_os(ast_validation_filepath);
        boost::write_graphviz(ast_os, ast, mfast::NodeInfoWriter(&node_buffers));

        if (config.plot_ast_graphs) {
            auto destination = ast_result_filepath;
            destination.replace_extension("png");
            std::system(("dot -Tpng -o " + destination.string() + " "
                         + ast_validation_filepath.string())
                            .c_str());
        }

        std::cout << "generated." << std::endl;
    } else {
        std::cout << "    ...validating AST generated against:\n        "
                  << ast_validation_filepath << "..." << std::endl;

        std::stringstream ast_ss;
        boost::write_graphviz(ast_ss, ast, mfast::NodeInfoWriter(&node_buffers));

        if (config.write_results | config.plot_ast_graphs) {
            std::ofstream token_os(ast_result_filepath);
            token_os << ast_ss.str();
        }

        if (config.plot_ast_graphs) {
            std::system(("dot -Tpng -O " + ast_result_filepath.string()).c_str());
        }

        std::ifstream     ast_is(ast_validation_filepath);
        std::stringstream buffer;
        buffer << ast_is.rdbuf();

        std::string result   = ast_ss.str();
        std::string expected = buffer.str();

        if (result != expected) return TestResult::SYNTAX_PARSING_FAILURE;

        std::cout << "validated." << std::endl;
    }

    return TestResult::SUCCESS;
}

void run_tests(TestConfig config = {}) {
    std::cout << "/-----------------------------\\\n"
              << "|  MATTFLOW REGRESSION TESTS  |\n"
              << "\\-----------------------------/\n"
              << std::endl;

    if (config.plot_ast_graphs && std::system("dot --version") != 0) {
        std::cout << "WARNING : plot_ast_graphs set true but graphviz is not installed."
                  << std::endl;
    }

    size_t successes               = 0;
    size_t lexing_failures         = 0;
    size_t syntax_parsing_failures = 0;
    size_t unspecified_failures    = 0;

    for (const auto& test_case : recurse_directory("samples")) {
        if (!test_case.is_regular_file()) continue;

        std::cout << "\n-------- " << test_case.path() << " --------" << std::endl;
        switch (run_test(test_case.path(), config)) {
            case TestResult::SUCCESS:
                std::cout << "\nResult : SUCCESS" << std::endl;
                successes += 1;
                break;
            case TestResult::LEXING_FAILURE:
                std::cout << "\nResult : LEXING_FAILURE" << std::endl;
                lexing_failures += 1;
                break;
            case TestResult::SYNTAX_PARSING_FAILURE:
                std::cout << "\nResult : SYNTAX_PARSING_FAILURE" << std::endl;
                syntax_parsing_failures += 1;
                break;
            case TestResult::UNSPECIFIED_FAILURE:
            default:
                std::cout << "\nResult : UNSPECIFIED_FAILURE" << std::endl;
                unspecified_failures += 1;
                break;
        }
        std::cout << "\n--------------------";
        for (const auto& _ : test_case.path().string()) std::cout << "-";
        std::cout << std::endl;
    }

    std::cout << "\n\nTests complete!" << std::endl;

    std::cout << "\n----------- SUMMARY -----------\n" << std::endl;
    std::cout << "  Unspecified Failures    : " << unspecified_failures << std::endl;
    std::cout << "  Lexing Failures         : " << lexing_failures << std::endl;
    std::cout << "  Syntax Parsing Failures : " << syntax_parsing_failures << std::endl;
    std::cout << "  Successes               : " << successes << std::endl;
    std::cout << "\n-------------------------------" << std::endl;
}

int main() {
    run_tests({ .plot_ast_graphs = true });
}
