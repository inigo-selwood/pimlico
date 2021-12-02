#pragma once

#include <string>

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

    void skip_space(const bool &overflow);
    void skip_line(const bool &overflow);
    void skip_whitespace();

    std::string line_text(const bool &escaped) const;
    std::string line_text(const long &line_number, const bool &escaped) const;

private:

    std::vector<std::pair<long, int>> line_indices;

    std::string text;

    long length;

    char get_character() const;

    void increment();
    void increment(const int &steps);

};
