#pragma once

#include <vector>
#include <utility>
#include <string>

#include "../text_position/text_position.hpp"

class ErrorBuffer {

public:

    friend std::ostream &operator<<(std::ostream &stream, 
            const ErrorBuffer &buffer);

    void add(const std::string &message, const TextPosition &position);

    void print(std::ostream &stream) const;

private:

    std::vector<std::pair<std::string, TextPosition>> errors;

};
