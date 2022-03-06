#pragma once

#include <iostream>
#include <string>
#include <utility>
#include <vector>

namespace Pimlico {

class ParseBuffer {

public:

    struct Position {

    public:

        long index;

        long line;
        int column;

        friend std::ostream &operator<<(std::ostream &stream,
                const Position &position);

        bool operator==(const Position &other) const;

        Position();
        Position(const bool &empty);

        void print(std::ostream &stream) const;

    };

    class Error {

    public:

        class Instance  {

        public:

            virtual void print(std::ostream &stream) const = 0;

            virtual ~Instance() = default;

        };

        friend std::ostream &operator<<(std::ostream &stream,
                const Error &position);

        operator bool() const;

        void add(const std::string &source, const std::string &text);
        void add(const std::string &source,
                const std::string &text,
                const ParseBuffer &buffer);
        void add(const std::string &source,
                const std::string &text,
                const ParseBuffer &buffer,
                const Position &position);

        void print(std::ostream &stream) const;

        ~Error();

    private:

        std::vector<Instance *> instances;

    };

    Position position;

    ParseBuffer(const std::string &text);

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

}; // Namespace Pimlico
