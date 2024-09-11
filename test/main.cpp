#include "stdafx.h"

#include "lex/lexer.h"

#include "ast/parse.h"

static char* read_file_to_string(const std::filesystem::path& path) {
    std::filesystem::path abs_path = std::filesystem::absolute(path);
    if (!std::filesystem::exists(abs_path)) return nullptr;

    // Open file, if we can't then fail.
    FILE* file = fopen(abs_path.string().data(), "rb");
    if (file == nullptr) return nullptr;

    // NOTE: Only support files up to 4GB in size. Would need some paging approach
    //       for larger files.

    // Get length of file contents in bytes.
    fseek(file, 0, SEEK_END);
    uint32_t length = static_cast<uint32_t>(ftell(file));
    fseek(file, 0, SEEK_SET);

    // Reserve memory in buffer.
    char* buffer = new char[length + 1];

    // Read data into buffer.
    fread(buffer, 1, length, file);
    buffer[length] = '\0';

    // Close file.
    fclose(file);

    return buffer;
}

void test_lexer() {
    char* sample_0 = read_file_to_string("samples/hello_world.mf");
    if (sample_0 == nullptr) {
        std::cout << "Could not read samples/hello_world.mf." << std::endl;
        exit(1);
    }

    mf::SourceView source_view;
    source_view.source       = std::string_view(sample_0);
    source_view.file_id      = 0;
    source_view.start_line   = 0;
    source_view.start_column = 0;
    source_view.end_line     = std::count_if(
        source_view.source.begin(),
        source_view.source.end(),
        [](char c) { return c == '\n'; }
    );
    source_view.end_column = std::find_if(
                                 source_view.source.rbegin(),
                                 source_view.source.rend(),
                                 [](char c) { return c == '\n'; }
                             )
                             - source_view.source.rbegin();

    mflex::Tokens tokens;
    mflex::parse(source_view, tokens);

    assert(
        static_cast<int16_t>(tokens[0].type)
        == static_cast<int16_t>(mflex::TokenType::IDENTIFIER)
    );
    assert(
        static_cast<int16_t>(tokens[1].type)
        == static_cast<int16_t>(mflex::TokenType::ASSIGN_TYPE)
    );
    assert(
        static_cast<int16_t>(tokens[2].type)
        == static_cast<int16_t>(mflex::TokenType::ASSIGN_VALUE)
    );
    assert(
        static_cast<int16_t>(tokens[3].type)
        == static_cast<int16_t>(mflex::TokenType::LEFT_PAREN)
    );
    assert(
        static_cast<int16_t>(tokens[4].type)
        == static_cast<int16_t>(mflex::TokenType::RIGHT_PAREN)
    );
    assert(
        static_cast<int16_t>(tokens[5].type)
        == static_cast<int16_t>(mflex::TokenType::ARROW)
    );
    assert(
        static_cast<int16_t>(tokens[6].type)
        == static_cast<int16_t>(mflex::TokenType::LEFT_BRACE)
    );
    assert(
        static_cast<int16_t>(tokens[7].type)
        == static_cast<int16_t>(mflex::TokenType::PRINT)
    );
    assert(
        static_cast<int16_t>(tokens[8].type)
        == static_cast<int16_t>(mflex::TokenType::LEFT_PAREN)
    );
    assert(
        static_cast<int16_t>(tokens[9].type)
        == static_cast<int16_t>(mflex::TokenType::STRING)
    );
    assert(
        static_cast<int16_t>(tokens[10].type)
        == static_cast<int16_t>(mflex::TokenType::RIGHT_PAREN)
    );
    assert(
        static_cast<int16_t>(tokens[11].type)
        == static_cast<int16_t>(mflex::TokenType::NUMBER)
    );
    assert(
        static_cast<int16_t>(tokens[12].type)
        == static_cast<int16_t>(mflex::TokenType::RIGHT_BRACE)
    );
}

void test_parser() {
    char* sample_0 = read_file_to_string("samples/primary.mf");
    if (sample_0 == nullptr) {
        std::cout << "Could not read samples/primary.mf." << std::endl;
        exit(1);
    }

    mf::SourceView source_view;
    source_view.source       = std::string_view(sample_0);
    source_view.file_id      = 0;
    source_view.start_line   = 0;
    source_view.start_column = 0;
    source_view.end_line     = std::count_if(
        source_view.source.begin(),
        source_view.source.end(),
        [](char c) { return c == '\n'; }
    );
    source_view.end_column = std::find_if(
                                 source_view.source.rbegin(),
                                 source_view.source.rend(),
                                 [](char c) { return c == '\n'; }
                             )
                             - source_view.source.rbegin();

    mflex::Tokens tokens;
    mflex::parse(source_view, tokens);

    mfast::AST                  ast;
    mfast::NodeBuffers          node_buffers;
    mftype::IdentifierTypeTable type_table;
    mfast::parse(tokens, ast, node_buffers, type_table);

    bool skip_first = true;
    for (auto v : boost::make_iterator_range(boost::vertices(ast))) {
        if (skip_first) {
            skip_first = false;
            continue;
        }

        mfast::NodeInfo __node_info
            = node_buffers.node_info[node_buffers.vertex_node_map[v]];
        std::visit(
            [](auto&& node_info) {
                using T = std::decay_t<decltype(node_info)>;
                if constexpr (std::is_same_v<T, mfast::BoolNode>) {
                    if (node_info.value) {
                        std::cout << "true ";
                    } else {
                        std::cout << "false ";
                    }
                } else if constexpr (std::is_same_v<T, mfast::NumberNode>) {
                    if (node_info.value.is_floating_point()) {
                        std::cout << node_info.value.template as<float>() << " ";
                    } else {
                        std::cout << node_info.value.template as<int>() << " ";
                    }
                } else if constexpr (std::is_same_v<T, mfast::StringNode>) {
                    std::cout << node_info.value << " ";
                } else if constexpr (std::is_same_v<T, mfast::NullNode>) {
                    std::cout << "null ";
                }
            },
            __node_info
        );

        std::cout << std::endl;
    }
}

int main() {
    std::cout << "Running mattflow tests..." << std::endl;

    test_lexer();

    test_parser();

    std::cout << "Tests passed!" << std::endl;
}
