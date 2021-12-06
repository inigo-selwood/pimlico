#pragma once

#include <pimlico.hpp>

namespace Pimlico {

class Range : public Term {

public:

    std::array<char, 2> value;

    Range();

    static Range *parse(Buffer::Parse &buffer, Buffer::Error &errors);

    void print(std::ostream &stream) const override;

};

}; // Namespace Pimlico
