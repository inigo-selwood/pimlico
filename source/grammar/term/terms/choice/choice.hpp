#pragma once

#include <pimlico.hpp>

namespace Pimlico {

class Choice : public Term {

public:

    std::vector<Term *> values;

    Choice();

    static Term *parse(ParseBuffer &buffer, ParseBuffer::Error &errors);

    void print(std::ostream &stream) const override;

    virtual ~Choice();

};

}; // Namespace Pimlico
