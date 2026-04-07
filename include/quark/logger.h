#pragma once
#include <iostream>
#include <string_view>
#include <cstdlib>
#include <chrono>
#include <sstream>
#include "colors.h" 

using namespace quark::colors;

namespace quark::logger {

inline double get_time_ms() {
    using namespace std::chrono;
    static auto start_time = high_resolution_clock::now();
    auto now = high_resolution_clock::now();
    return duration<double, std::milli>(now - start_time).count();
}

template<typename... Args>
inline std::string format(Args&&... args) {
    std::ostringstream oss;
    (oss << ... << args);
    return oss.str();
}

template<typename... Args>
inline void log_info(Args&&... args) {
    std::cout << "(" << get_time_ms() << " ms): " << format(std::forward<Args>(args)...) << '\n';
}

template<typename... Args>
inline void log_warn(Args&&... args) {
    set_color(YELLOW);
    std::cout << "(" << get_time_ms() << " ms): " << format(std::forward<Args>(args)...) << '\n';
    reset_color();
}

template<typename... Args>
inline void log_error(Args&&... args) {
    set_color(RED);
    std::cerr << "(" << get_time_ms() << " ms): " << format(std::forward<Args>(args)...) << '\n';
    reset_color();
}

template<typename... Args>
[[noreturn]] inline void fatal(Args&&... args) {
    log_error(std::forward<Args>(args)...);
    std::exit(1);
}

} // namespace quark
