#include "stdafx.h"

#include "profile.h"

mfcli::CompilationTime
mfcli::operator+(const mfcli::CompilationTime& lhs, const mfcli::CompilationTime& rhs) {
    return { lhs.lex_dur + rhs.lex_dur,
             lhs.ast_dur + rhs.ast_dur,
             lhs.backend_dur + rhs.backend_dur };
}

mfcli::Profile mfcli::Profile::from(const mattflow::SourceView& source_view) {
    return Profile{ {}, source_view.end_line - source_view.start_line + 1 };
}

mfcli::Profile mfcli::operator+(const mfcli::Profile& lhs, const mfcli::Profile& rhs) {
    return { lhs.times + rhs.times, lhs.lines + rhs.lines };
}

static char* make_time_string(std::chrono::nanoseconds time) {
    auto  raw_time = time.count();
    char* result   = new char[10];
    if (raw_time > 1e10) {
        snprintf(result, 10, "%5ld%s", static_cast<uint64_t>(raw_time / 1e9), "s");
    } else if (raw_time > 1e7) {
        snprintf(result, 10, "%5ld%s", static_cast<uint64_t>(raw_time / 1e6), "ms");
    } else if (raw_time > 1e4) {
        snprintf(result, 10, "%5ld%s", static_cast<uint64_t>(raw_time / 1e3), "us");
    } else {
        snprintf(result, 10, "%5ld%s", static_cast<uint64_t>(raw_time), "ns");
    }

    return result;
};

void mfcli::print_profile(const mfcli::Profile& profile) {
    auto times = profile.times;

    // Get time in seconds.
    double total_time = static_cast<double>(
        times.lex_dur.count() + times.ast_dur.count() + times.backend_dur.count()
    );
    total_time /= 1.e9;

    uint64_t lines_per_second = profile.lines / total_time;

    setlocale(LC_NUMERIC, "");
    printf("    Lexing             :     %s\n", make_time_string(times.lex_dur));
    printf("    Syntactic Analysis :     %s\n", make_time_string(times.ast_dur));
    printf("    LLVM Backend       :     %s\n", make_time_string(times.backend_dur));
    printf("    Lines per Second   :  %'10ld\n\n", lines_per_second);
}
