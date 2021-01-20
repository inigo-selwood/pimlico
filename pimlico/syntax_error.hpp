#pragma once

namespace Pimlico {

struct SyntaxError {

public:

    unsigned long line;
    unsigned int column;

    std::string message;
    std::string text;

};

};
