#ifndef CPP_LOG_WATCH_H
#define CPP_LOG_WATCH_H

#include <chrono>
#include <cstdint>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <string>
#include <type_traits>

/*
 * This is an ready-to-run library, so time precision can be arranged to user unit
 * All the definitions and class were copied from other projects
 * In this program, millisecond precision is needed to show execution time of thread functions
 */

// A C++11 constexpr function template for counting decimals needed for
// selected precision.
template<std::size_t V, std::size_t C = 0,
        typename std::enable_if<(V < 10), int>::type = 0>
constexpr std::size_t log10ish() {
    return C;
}

template<std::size_t V, std::size_t C = 0,
        typename std::enable_if<(V >= 10), int>::type = 0>
constexpr std::size_t log10ish() {
    return log10ish<V / 10, C + 1>();
}

// A class to support using different precisions, chrono clocks and formats
template<class Precision = std::chrono::seconds,
        class Clock = std::chrono::system_clock>
class log_watch {
public:
    // some convenience typedefs and "decimal_width" for sub second precisions
    using precision_type = Precision;
    using ratio_type = typename precision_type::period;
    using clock_type = Clock;
    static constexpr auto decimal_width = log10ish<ratio_type{}.den>();

    static_assert(ratio_type{}.num <= ratio_type{}.den,
                  "Only second or sub second precision supported");
    static_assert(ratio_type{}.num == 1, "Unsupported precision parameter");

    // default format: "%Y-%m-%dT%H:%M:%S"
    log_watch(const std::string& format = "%FT%T") : m_format(format) {}

    template<class P, class C>
    friend std::ostream& operator<<(std::ostream&, const log_watch<P, C>&);

private:
    std::string m_format;
};

template<class Precision, class Clock>
std::ostream& operator<<(std::ostream& os, const log_watch<Precision, Clock>& lw) {
    // get current system clock
    auto time_point = Clock::now();

    // extract std::time_t from time_point
    std::time_t t = Clock::to_time_t(time_point);

    // output the part supported by std::tm
    os << std::put_time(std::localtime(&t), lw.m_format.c_str());

    // only involve chrono duration calc for displaying sub second precisions
    if(lw.decimal_width) { // if constexpr( ... in C++17
        // get duration since epoch
        auto dur = time_point.time_since_epoch();

        // extract the sub second part from the duration since epoch
        auto ss =
                std::chrono::duration_cast<Precision>(dur) % std::chrono::seconds{1};

        // output the sub second part
        os << std::setfill('0') << std::setw(lw.decimal_width) << ss.count();
    }

    return os;
}

#endif //CPP_LOG_WATCH_H
