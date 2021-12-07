#pragma once

#include <string>
#include <utility>
#include <vector>

#include "../position/position.hpp"

namespace Pimlico {

namespace Buffer {

class Parse {

public:

    std::vector<std::pair<long, char>> indextations;

    Buffer::Position position;

    Parse(const std::string &text);

    bool finished() const;

    char peek() const;
    bool peek(const char &) const;
    bool peek(const std::string &) const;

    void skip_space();
    void skip_line();

    char read();
    bool read(const char &);
    bool read(const std::string &);

private:

    std::string text;

    long long length;

    char get_character() const;

    void increment();
    void increment(const int &steps);

};

}; // Namespace Buffer

}; // Namespace Pimlico
