#pragma once

#include <pimlico.hpp>

namespace Pimlico {

namespace Buffer {

class Message : public Error::Instance {

public:

    Message(const std::string &source, const std::string &text);

    void print(std::ostream &stream) const override;

private:

    std::string source;
    std::string text;

};

}; // Namespace Buffer

}; // Namespace Pimlico
