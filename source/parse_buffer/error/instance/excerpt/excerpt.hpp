#pragma once

#include <pimlico.hpp>

namespace Pimlico {

class Excerpt : public ParseBuffer::Error::Instance {

public:

    Excerpt(const std::string &source,
            const std::string &message,
            const ParseBuffer &buffer);
    Excerpt(const std::string &source,
            const std::string &message,
            const ParseBuffer &buffer,
            const ParseBuffer::Position &position);

    void print(std::ostream &stream) const override;

private:

    std::string source;
    std::string message;
    std::string text;

    ParseBuffer::Position position;

};

}; // Namespace Pimlico
