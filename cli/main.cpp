#include "stdafx.h"

#include <argparse/argparse.hpp>

#include "ast/debug.h"
#include "ast/parse.h"
#include "lex/lexer.h"

#include "exit_codes.h"

#define MATTFLOW_VERSION_STRING "mattflow v" + std::string(MATTFLOW_VERSION)

struct CompilationTime {
    std::chrono::nanoseconds lex_dur;
    std::chrono::nanoseconds ast_dur;
};

CompilationTime operator+(const CompilationTime& lhs, const CompilationTime& rhs) {
    return { lhs.lex_dur + rhs.lex_dur, lhs.ast_dur + rhs.ast_dur };
}

static std::filesystem::path make_dot_filepath(const std::filesystem::path& path) {
    return path.filename().replace_extension(".dot");
}

CompilationTime
parse_file(const std::string& file, const argparse::ArgumentParser& args) {
    mattflow::SourceView source_view;
    if (!mattflow::SourceView::from_filepath(file, source_view)) {
        std::cout << "Could not read file at path:\n    " << file << std::endl;
        exit(FILE_UNREADABLE);
    }

    CompilationTime duration;

    // Lexing

    auto lex_start = std::chrono::high_resolution_clock::now();

    mflex::Tokens tokens;
    mflex::parse(source_view, tokens);

    duration.lex_dur = std::chrono::high_resolution_clock::now() - lex_start;

    // Syntactic Analysis

    auto ast_start = std::chrono::high_resolution_clock::now();

    mfast::AST                  ast;
    mfast::NodeBuffers          node_buffers;
    mftype::IdentifierTypeTable type_table;
    mfast::parse(tokens, ast, node_buffers, type_table);

    duration.ast_dur = std::chrono::high_resolution_clock::now() - ast_start;

    if (args["--plot-ast"] == true) {
        auto dot_filepath = make_dot_filepath(file);

        std::ofstream ast_os(dot_filepath);
        boost::write_graphviz(ast_os, ast, mfast::NodeInfoWriter(&node_buffers));

        (void)std::system(("dot -Tpng -O " + dot_filepath.string()).c_str());

        std::filesystem::remove(dot_filepath);
    }

    return duration;
}

int main(int argc, char** argv) {
    argparse::ArgumentParser cli("mattflow", MATTFLOW_VERSION_STRING);

    cli.add_description("mattflow compiler, based on LLVM.");

    cli.add_argument("--verbose").help("Increases verbosity of the compiler.").flag();

    cli.add_argument("--profile")
        .help("If this flag is set, the compiler emits profiler information.")
        .flag();

    cli.add_argument("--interactive")
        .help("If this flag is set, the user is solicited with a file to compile.")
        .flag();

    cli.add_argument("input files").remaining();

    cli.add_group("Compiler options");

    cli.add_argument("-o", "--output")
        .help("The name of the executable resulting from compilation.")
        .default_value("a.out");

    cli.add_argument("--dry-run")
        .help("If this flag is set, the compiler doesn't create an executable.")
        .flag();

    cli.add_group("Debugging options");

    cli.add_argument("--plot-ast")
        .help("If this flag is set, the AST is plotted as a PNG.")
        .flag();

    cli.parse_args(argc, argv);

    if (cli["--plot-ast"] == true) {
        if (std::system("dot --version") != 0) {
            std::cout << "WARNING : --plot-ast set true but graphviz is not installed."
                      << std::endl;
            return GRAPHVIZ_UNAVAILABLE;
        }
        std::cout << std::endl;
    }

    std::vector<CompilationTime> durations;
    auto input_files = cli.get<std::vector<std::string>>("input files");

    if (input_files.size() == 0 && cli["--interactive"] == false) {
        std::cout << "ERROR : no input files provided and not running compiler "
                  << "interactively.\nProvide at least one input file or set "
                  << "--interactive." << std::endl;
        return NO_FILES_PROVIDED;
    }

    if (cli["--interactive"] == true) {
        std::cout << "Name a file to compile [samples/profile/1k_infix.mf] : ";
        std::string input;
        std::cin >> input;

        input_files = { input };
    }

    for (auto& input_file : input_files) {
        std::cout << input_file << std::endl;

        durations.emplace_back(parse_file(input_file, cli));

        if (cli["--profile"] == true) {
            std::cout << "    Lexing: " << durations.back().lex_dur.count() / 1000
                      << "us\n    Syntactic Analysis: "
                      << durations.back().ast_dur.count() / 1000 << "us" << std::endl;
        }
    }

    if (cli["--profile"] == true) {
        auto total_duration
            = std::accumulate(durations.begin(), durations.end(), CompilationTime{});

        std::cout << "\nTotal Compile Time:\n    Lexing: "
                  << total_duration.lex_dur.count() / 1000
                  << "us\n    Syntactic Analysis: "
                  << total_duration.ast_dur.count() / 1000 << "us" << std::endl;
    }

    return 0;
}
