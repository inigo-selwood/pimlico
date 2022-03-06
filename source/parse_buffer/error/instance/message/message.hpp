#pragma once

#include <pimlico.hpp>

namespace Pimlico {

class Message : public ParseBuffer::Error::Instance {

public:

    Message(const std::string &source, const std::string &text);

    void print(std::ostream &stream) const override;

private:

    std::string source;
    std::string text;

};

}; // Namespace Pimlico
