#pragma once

#include <pimlico.hpp>

namespace Pimlico {

class Choice : public Term {

public:

    std::vector<Term *> values;

    Choice();

    static Term *parse(Buffer::Parse &buffer, Buffer::Error &errors);

    void print(std::ostream &stream) const override;

    virtual ~Choice();

};

}; // Namespace Pimlico
