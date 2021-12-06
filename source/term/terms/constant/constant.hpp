#pragma once

#include <pimlico.hpp>

namespace Pimlico {

class Constant : public Term {

public:

    std::string value;

    Constant();

    static Constant *parse(Buffer::Parse &buffer, Buffer::Error &errors);

    void print(std::ostream &stream) const override;

};

}; // Namespace Pimlico
