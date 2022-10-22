#ifndef LOG_H
#define LOG_H

#include <iostream>

template<typename T>
concept Printable = requires(std::ostream& out, T const& t) {
    out << t;
};

template<Printable... Args>
void print_logline(Args ...args) {
    (std::cout << ... << args) << std::endl;
}

#endif