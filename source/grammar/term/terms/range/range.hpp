#pragma once

#include <pimlico.hpp>

namespace Pimlico {

class Range : public Term {

public:

    std::array<char, 2> value;

    Range();

    static Range *parse(ParseBuffer &buffer, ParseBuffer::Error &errors);

    void print(std::ostream &stream) const override;

};

}; // Namespace Pimlico
