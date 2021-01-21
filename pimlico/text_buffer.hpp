#pragma once

namespace Pimlico {

class TextBuffer {

public:

    unsigned int column;

    unsigned long line;

    char read(const char &character) {
        return 0;
    }

    char read() {
        return 0;
    }

    char peek(const char &character) const {
        return 0;
    }

    char peek() const {
        return 0;
    }

    std::string line_text() const {
        return "";
    }

    void skip_space() {}
    void skip_whitespace() {}

    unsigned int line_indentation(const unsigned int &line_number) const {}

    bool end_reached() const {
        return true;
    }

};

};
