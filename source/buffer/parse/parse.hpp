#pragma once

#include <string>
#include <utility>
#include <vector>

#include "../position/position.hpp"

namespace Pimlico {

namespace Buffer {

class Parse {

public:

    Buffer::Position position;

    Parse(const std::string &text);

    bool finished() const;

    char peek() const;
    bool peek(const char &) const;
    bool peek(const std::string &) const;

    void skip_line();
    void skip_space();
    void skip_whitespace();

    char read();
    bool read(const char &);
    bool read(const std::string &);

    std::string line_text(long line_number = 0) const;
    char line_indentation(long line_number = 0) const;

private:

    std::vector<std::pair<long, char>> indextations;

    std::string text;

    long long length;

    char get_character() const;

    void increment();
    void increment(const int &steps);

};

}; // Namespace Buffer

}; // Namespace Pimlico
