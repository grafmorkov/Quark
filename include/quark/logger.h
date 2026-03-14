#ifndef QUARK_LOGGER_H
#define QUARK_LOGGER_H

#include <iostream>
#include <string_view>
#include <cstdlib>
#include <chrono>
#include "colors.h" 

using namespace quark::colors;

namespace quark {

inline double get_time_ms() {
    using namespace std::chrono;
    static auto start_time = high_resolution_clock::now();
    auto now = high_resolution_clock::now();
    return duration<double, std::milli>(now - start_time).count();
}

inline void log_info(std::string_view msg) {
    std::cout << "(" << get_time_ms() << " ms) INFO: " << msg << '\n';
}

inline void log_warn(std::string_view msg) {
    set_color(YELLOW);
    std::cout << "(" << get_time_ms() << " ms) WARN: " << msg << '\n';
    reset_color();
}

inline void log_error(std::string_view msg) {
    set_color(RED);
    std::cerr << "(" << get_time_ms() << " ms) ERROR: " << msg << '\n';
    reset_color();
}

[[noreturn]] inline void fatal(std::string_view msg) {
    log_error(msg);
    std::exit(1);
}

} // namespace quark

#endif // QUARK_LOGGER_H