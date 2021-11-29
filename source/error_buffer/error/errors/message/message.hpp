#pragma once

#include <string>

#include "../../error.hpp"

class Message : public Error {

public:

    friend std::ostream &operator<<(std::ostream &stream,
            const Message &error);

    Message(const std::string &text);

    void print(std::ostream &stream) const override;

private:

    std::string text;

};
