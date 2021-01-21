#pragma once

#include <functional>
#include <string>
#include <vector>

namespace Pimlico {

class TextBuffer {

public:

    std::function<int(TextBuffer &)> comment_skip_function;

    unsigned int column_number;

    unsigned long line_number;

    TextBuffer(const std::string &text);

    bool end_reached() const;

    void increment(const unsigned int steps);

    std::string line_text() const;
    unsigned int line_indentation(const unsigned int &line_number) const;

    char peek(const char &character) const;
    char peek() const;

    char read(const char &character);
    char read();

    void skip_space();
    void skip_whitespace();
    void skip_line();

private:

    std::string text;

    unsigned long index;

    inline char get_character() const;

};

TextBuffer::TextBuffer(const std::string &text) {
    this->text = text;

    index = 0;
}

bool TextBuffer::end_reached() const {
    return index >= text.length();
}

void TextBuffer::increment(const unsigned int steps = 1) {
    for(long step = 0; step < steps; step += 1) {
        if(index == text.length())
            return;

        index += 1;
        column_number += 1;

        if(text[index - 1] == '\n') {
            line_number += 1;
            column_number = 1;
        }
    }
}

std::string TextBuffer::line_text() const {
    unsigned long start_index = index;
    while(true) {
        if(start_index == 0 || text[start_index - 1] == '\n')
            break;
        start_index -= 1;
    }

    unsigned long end_index = index;
    while(true) {
        if(end_index + 1 == text.length() || text[end_index] == '\n')
            break;
        end_index += 1;
    }

    const unsigned long length = end_index - start_index;
    return text.substr(start_index, length);
}

unsigned int TextBuffer::line_indentation(const unsigned int &line_number) const { return 0; }

char TextBuffer::peek(const char &character) const {
    return get_character() == character;
}

char TextBuffer::peek() const {
    return get_character();
}

char TextBuffer::read(const char &character) {
    if(get_character() == character) {
        increment();
        return true;
    }
    return false;
}

char TextBuffer::read() {
    const char result = get_character();
    increment();
    return result;
}

void TextBuffer::skip_space() {
    long start_index = index;
    while(index < text.length()) {

        // Handle spaces and tabs
        const auto character = get_character();
        if(character == ' ') {
            index += 1;
            column_number += 1;
        }
        else if(character == '\r')
            index += 1;
        else if(character == '\t') {
            index += 1;

            const int remainder = column_number % 5;
            column_number += (5 - remainder);
        }

        // Handle comments
        else if(comment_skip_function != nullptr &&
                comment_skip_function(*this))
            continue;

        // Failing the above, break out
        else
            break;
    }
}

void TextBuffer::skip_whitespace() {
    long start_index = index;
    while(true) {
        skip_space();
        if(read('\n') == false)
            break;
    }
}

void TextBuffer::skip_line() {
    const long start_index = index;
    if(index < text.length() && text[index - 1] == '\n')
        index += 1;

    while(index < text.length() && text[index - 1] != '\n')
        index += 1;

    if(index >= text.length())
        return;
    column_number = 1;
    line_number += 1;
}

inline char TextBuffer::get_character() const {
    return (index < text.length()) ? text[index] : 0;
}

};
