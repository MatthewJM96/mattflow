#include "stdafx.h"

#include <argparse/argparse.hpp>

#include "exit_codes.h"
#include "parse.h"
#include "profile.h"

#define MATTFLOW_VERSION_STRING "mattflow v" + std::string(MATTFLOW_VERSION)

int main(int argc, char** argv) {
    // Set up argument parser with our various options.
    argparse::ArgumentParser cli("mattflow", MATTFLOW_VERSION_STRING);

    cli.add_description("mattflow compiler, based on LLVM.");

    cli.add_argument("--verbose").help("Increases verbosity of the compiler.").flag();

    cli.add_argument("--profile")
        .help("If this flag is set, the compiler emits profiler information.")
        .flag();

    cli.add_argument("--interactive")
        .help("If this flag is set, the user is solicited with a file to compile.")
        .flag();

    cli.add_argument("--emit-command")
        .help(
            "If this flag is set, the command which called this program was invoked is "
            "emitted."
        )
        .flag();

    cli.add_argument("input files").remaining();

    cli.add_group("Compiler options");

    cli.add_argument("-o", "--output")
        .help("The name of the executable resulting from compilation.")
        .default_value("a.out");

    cli.add_argument("--log-dir")
        .help("The path of the directory into which logs will be stored.")
        .default_value("./log/");

    cli.add_argument("--dry-run")
        .help("If this flag is set, the compiler doesn't create an executable.")
        .flag();

    cli.add_group("Debugging options");

    cli.add_argument("--plot-ast")
        .help("If this flag is set, the AST is plotted as a PNG.")
        .flag();

    // Parse command-line.
    try {
        cli.parse_args(argc, argv);
    } catch (const std::exception& e) {
        std::cout << "ERROR : " << e.what() << std::endl;
        std::cout << cli << std::endl;
        return CLI_PARSE_ERROR;
    }

    // Get settings in useful form.
    bool profile      = cli["--profile"] == true;
    bool interactive  = cli["--interactive"] == true;
    bool emit_command = cli["--emit-command"] == true;
    bool plot_ast     = cli["--plot-ast"] == true;

    std::filesystem::path log_dir = cli.get("--log-dir");

    std::vector<std::string> input_files = {};
    try {
        input_files = cli.get<std::vector<std::string>>("input files");
    } catch (const std::logic_error&) {
        // Empty.
    }

    // Emit command-line string if requested.
    if (emit_command) {
        for (int i = 0; i < argc; ++i) std::cout << argv[i] << " ";
        std::cout << std::endl << std::endl;
    }

    // Ensure the log directory exists.
    std::filesystem::create_directories(log_dir);

    // Check we have graphviz if we need it.
    if (plot_ast) {
        if (std::system("dot -V") != 0) {
            std::cout << "WARNING : --plot-ast set true but graphviz is not installed."
                      << std::endl;
            return GRAPHVIZ_UNAVAILABLE;
        }
        std::cout << std::endl;
    }

    // We must have a route to get a file to compile one way or another.
    if (input_files.size() == 0 && !interactive) {
        std::cout << "ERROR : no input files provided and not running compiler "
                  << "interactively.\nProvide at least one input file or set "
                  << "--interactive." << std::endl;
        return NO_FILES_PROVIDED;
    }

    // If we are getting the file interactively then do so.
    if (interactive) {
        std::cout << "Name a file to compile [test/samples/profile/1k_infix.mf] : ";
        std::string input;
        std::getline(std::cin, input);

        if (input == "") {
            input = "test/samples/profile/1k_infix.mf";
        }

        input_files = { input };
    }

    // Output separation, starting file parsing.
    std::cout << std::endl;

    // Parse each input file in turn.
    std::vector<mfcli::Profile> profiles;
    for (auto& input_file : input_files) {
        profiles.emplace_back(mfcli::parse_file(input_file, plot_ast, log_dir));

        if (profile) mfcli::print_profile(profiles.back());
    }

    // Emit profiling information if requested.
    if (profile) {
        mfcli::Profile total_profile
            = std::accumulate(profiles.begin(), profiles.end(), mfcli::Profile{});

        if (profile) {
            std::cout << "Total Profile:" << std::endl;
            mfcli::print_profile(total_profile);
        }
    }

    return 0;
}
