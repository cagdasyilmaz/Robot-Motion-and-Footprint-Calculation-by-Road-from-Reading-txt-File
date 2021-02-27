//
// Created by phsmm on 2/25/21.
//

#ifndef CPP_LOCKED_STREAM_H
#define CPP_LOCKED_STREAM_H

#include <iostream>
#include <mutex>

class LockedStream
{
    // static mutex for controlling access multi-thread output operations
    static std::mutex s_out_mutex;

public:
    // Public initializer stream
    /*
     * @lock_ : unique_lock mutex
     * @stream:_ : output stream which will be sent to console
     */
    LockedStream(std::ostream& stream)
        : lock_(s_out_mutex), stream_(&stream)
    {
    }

    /*
     * @lock_ : unique_lock mutex with perfect forwarding
     * @stream:_ : output stream which will be sent to console for other stream
     */
    LockedStream(LockedStream&& other)
        : lock_(std::move(other.lock_)), stream_(other.stream_)
    {
        other.stream_ = nullptr;
    }

    /*
     * Friend function for << operator which gives same functionality with standard library equivalent
     */
    friend LockedStream&& operator << (LockedStream&& s, std::ostream& (*arg)(std::ostream&))
    {
        (*s.stream_) << arg;
        return std::move(s);
    }

    /*
     * Friend function for other stream
     * with template definition
     */
    template <typename Arg>
    friend LockedStream&& operator << (LockedStream&& s, Arg && arg)
    {
        (*s.stream_) << std::forward<Arg>(arg);
        return std::move(s);
    }

private:
    std::unique_lock<std::mutex> lock_;  // mutex object for lock the cout thread
    std::ostream* stream_;               // stream obtained from thread operations.
};

#endif //CPP_LOCKED_STREAM_H
