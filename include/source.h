#ifndef __mattflow_source_h
#define __mattflow_source_h

namespace mattflow {
    using FileID = uint64_t;

    /**
     * @brief A view over source code, with start and end line + column tracked. file_id
     * represents the file in which this source is located.
     */
    struct SourceView {
        std::string_view source;
        FileID           file_id;
        uint64_t         start_line, end_line;
        uint64_t         start_column, end_column;

        static bool from_filepath(const std::filesystem::path& path, SourceView& view);
    };
}  // namespace mattflow
namespace mf = mattflow;

#endif  // __mattflow_source_h
