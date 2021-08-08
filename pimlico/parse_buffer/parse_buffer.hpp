#pragma once

#include <string>
#include <utility>
#include <vector>

#include "../text_position/text_position.hpp"

struct ParseBuffer {

public:

    TextPosition position;

    ParseBuffer(const std::string &text);

    char read();
    bool read(const char &character);
    bool read(const std::string &string);

    char peek() const;
    bool peek(const char &character) const;
    bool peek(const std::string &string) const;

    bool finished() const;

    void skip(const bool &overflow);

private:

    std::vector<std::pair<long, int>> line_indices;

    std::string text;

    long length;

    char get_character() const;

    void increment();
    void increment(const int &steps);

};
