#pragma once

#include <string>

#include "../parse/parse.hpp"
#include "../position/position.hpp"

namespace Pimlico {

namespace Buffer {

class Error {

public:

    friend std::ostream &operator<<(std::ostream &stream,
            const Error &position);

    void add(const std::string &text) {}
    void add(const std::string &text, const Parse &buffer) {}
    void add(const std::string &text,
            const Parse &buffer,
            const Position &position) {}

    void print(std::ostream &stream) const;

    ~Error();

private:

    std::vector<Error *> errors;

};

}; // Namespace Buffer

}; // Namespace Pimlico
