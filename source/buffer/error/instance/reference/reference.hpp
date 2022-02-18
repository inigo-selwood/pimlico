#pragma once

#include <pimlico.hpp>

namespace Pimlico {

namespace Buffer {

class Reference : public Error::Instance {

public:

    Reference(const std::string &source,
            const std::string &message,
            const Parse &buffer);
    Reference(const std::string &source,
            const std::string &message,
            const Parse &buffer,
            const Position &position);

    void print(std::ostream &stream) const override;

private:

    std::string source;
    std::string message;
    std::string text;

    Position position;

};

}; // Namespace Buffer

}; // Namespace Pimlico
