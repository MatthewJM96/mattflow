#include "stdafx.h"

#include "ast/debug.h"
#include "ast/parse/parse.h"
#include "backend/llvm.h"
#include "lex/lexer.h"

#include "exit_codes.h"

#include "parse.h"

static std::filesystem::path make_dot_filepath(
    const std::filesystem::path& path, const std::filesystem::path& log_dir
) {
    return log_dir / path.filename().replace_extension(".dot");
}

mfcli::Profile mfcli::parse_file(
    const std::string& file, bool plot_ast, const std::filesystem::path& log_dir
) {
    mattflow::SourceView source_view;
    if (!mattflow::SourceView::from_filepath(file, source_view)) {
        std::cout << "ERROR : Could not read file at path " << file << std::endl;
        exit(FILE_UNREADABLE);
    }

    std::cout << "Parsing " << file << std::endl;

    Profile profile = Profile::from(source_view);

    // Lexing

    auto lex_start = std::chrono::high_resolution_clock::now();

    mflex::Tokens tokens;
    mflex::parse(source_view, tokens);

    profile.times.lex_dur = std::chrono::high_resolution_clock::now() - lex_start;

    // Syntactic Analysis

    auto ast_start = std::chrono::high_resolution_clock::now();

    mfast::AST                  ast;
    mfast::NodeBuffers          node_buffers;
    mftype::IdentifierTypeTable type_table;
    mfvar::ScopeTree            scope_tree;
    mfast::parse(tokens, ast, node_buffers, type_table, scope_tree);

    profile.times.ast_dur = std::chrono::high_resolution_clock::now() - ast_start;

    if (plot_ast == true) {
        auto dot_filepath = make_dot_filepath(file, log_dir);

        std::ofstream ast_os(dot_filepath);
        boost::write_graphviz(ast_os, ast, mfast::NodeInfoWriter(&node_buffers));

        (void)std::system(("dot -Tpng -O " + dot_filepath.string()).c_str());

        std::filesystem::remove(dot_filepath);
    }

    // Backend

    auto backend_start = std::chrono::high_resolution_clock::now();

    mfbe::convert_module_to_llvm_ir(ast, node_buffers, type_table);

    profile.times.backend_dur
        = std::chrono::high_resolution_clock::now() - backend_start;

    return profile;
}
