#include "stdafx.h"

#include "source.h"

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

bool mattflow::SourceView::from_filepath(
    const std::filesystem::path& path, SourceView& view
) {
    if (!std::filesystem::is_regular_file(path)) return false;

    char* contents = read_file_to_string(path);
    if (contents == nullptr) return false;

    view.source       = std::string_view(contents);
    view.file_id      = 0;
    view.start_line   = 0;
    view.start_column = 0;
    view.end_line = std::count_if(view.source.begin(), view.source.end(), [](char c) {
        return c == '\n';
    });
    auto start_of_last_line_it
        = std::find_if(view.source.rbegin(), view.source.rend(), [](char c) {
              return c == '\n';
          });
    view.end_column = start_of_last_line_it - view.source.rbegin();

    return true;
}
