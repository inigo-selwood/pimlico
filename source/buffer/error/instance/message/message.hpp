#pragma once

#include <pimlico.hpp>

namespace Pimlico {

namespace Buffer {

class Message : public Instance {

public:

    Message(const std::string &text);

    void print(std::ostream &stream) const override;

private:

    std::string text;

};

}; // Namespace Buffer

}; // Namespace Pimlico
