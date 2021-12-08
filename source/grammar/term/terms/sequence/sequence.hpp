#pragma once

#include <pimlico.hpp>

namespace Pimlico {

class Sequence : public Term {

public:

    std::vector<Term *> values;

    Sequence();

    static Term *parse(Buffer::Parse &buffer, Buffer::Error &errors);

    void print(std::ostream &stream) const override;

    virtual ~Sequence();

};

}; // Namespace Pimlico
