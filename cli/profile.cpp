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

static std::string make_time_string(std::chrono::nanoseconds time) {
    std::string result;

    auto raw_time = time.count();
    if (raw_time > 1e10) {
        result = std::to_string(raw_time / 1e9) + "s";
    } else if (raw_time > 1e7) {
        result = std::to_string(raw_time / 1e6) + "ms";
    } else if (raw_time > 1e4) {
        result = std::to_string(raw_time / 1e3) + "us";
    } else {
        result = std::to_string(raw_time) + "ns";
    }

    return result;
};

static std::string format_with_commas(uint64_t value) {
    std::stringstream ss;
    ss.imbue(std::locale(""));
    ss << std::fixed << value;
    return ss.str();
}

void mfcli::print_profile(const mfcli::Profile& profile) {
    auto times = profile.times;

    // Get time in seconds.
    double total_time = static_cast<double>(
        times.lex_dur.count() + times.ast_dur.count() + times.backend_dur.count()
    );
    total_time /= 1.e9;

    uint64_t lines_per_second
        = static_cast<uint64_t>(static_cast<double>(profile.lines) / total_time);

    setlocale(LC_NUMERIC, "");
    std::cout << "    Lexing             :   " << make_time_string(times.lex_dur)
              << std::endl;
    std::cout << "    Syntactic Analysis :   " << make_time_string(times.ast_dur)
              << std::endl;
    std::cout << "    LLVM Backend       :   " << make_time_string(times.backend_dur)
              << std::endl;
    std::cout << "    Lines per Second   :   " << std::setw(10)
              << format_with_commas(lines_per_second) << std::endl;
}
