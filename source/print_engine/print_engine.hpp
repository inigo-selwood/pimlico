#pragma once

#include <string>
#include <map>

#include "../parse_buffer/parse_buffer.hpp"

namespace Pimlico {

class PrintEngine {

public:

    int indentation;

    PrintEngine(const std::string &text);

    void replace(const std::string &token, const std::string &value);

    std::string render();

private:

    std::string text;

    std::map<std::string, std::string> substitutions;

};

};
