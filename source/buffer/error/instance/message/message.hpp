#pragma once

#include <pimlico.hpp>

namespace Pimlico {

namespace Buffer {

class Message : public Instance {

public:

    friend std::ostream &operator<<(std::ostream &stream,
            const Message &message);

    Message(const std::string &text);

    void print(std::ostream &stream) const override;

private:

    std::string text;

};

}; // Namespace Buffer

}; // Namespace Pimlico
