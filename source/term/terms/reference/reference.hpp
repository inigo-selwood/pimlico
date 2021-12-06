#pragma once

#include <pimlico.hpp>

namespace Pimlico {

class Reference : public Term {

public:

    std::string name;

    Term *value;

    Reference();

    static Reference *parse(Buffer::Parse &buffer, Buffer::Error &errors);

    void print(std::ostream &stream) const override;

    virtual ~Reference();

};

}; // Namespace Pimlico
