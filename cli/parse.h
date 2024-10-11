#ifndef __mattflow_cli_parse_h
#define __mattflow_cli_parse_h

#include "profile.h"

namespace mattflow {
    namespace cli {
        Profile parse_file(
            const std::string& file, bool plot_ast, const std::filesystem::path& log_dir
        );
    }
}
namespace mfcli = mattflow::cli;

#endif  // __mattflow_cli_parse_h
