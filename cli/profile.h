#ifndef __mattflow_cli_profile_h
#define __mattflow_cli_profile_h

#include "source.h"

namespace mattflow {
    namespace cli {
        struct CompilationTime {
            std::chrono::nanoseconds lex_dur;
            std::chrono::nanoseconds ast_dur;
            std::chrono::nanoseconds backend_dur;
        };

        CompilationTime
        operator+(const CompilationTime& lhs, const CompilationTime& rhs);

        struct Profile {
            CompilationTime times;
            uint64_t        lines;

            static Profile from(const mattflow::SourceView& source_view);
        };

        Profile operator+(const Profile& lhs, const Profile& rhs);

        void print_profile(const Profile& profile);
    }  // namespace cli
}  // namespace mattflow
namespace mfcli = mattflow::cli;

#endif  // __mattflow_cli_profile_h
