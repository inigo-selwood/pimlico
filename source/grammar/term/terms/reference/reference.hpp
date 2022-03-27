#pragma once

#include <pimlico.hpp>

namespace Pimlico {

class Reference : public Term {

public:

    std::string name;

    Term *value;

    Reference();

    static Reference *parse(ParseBuffer &buffer, ParseBuffer::Error &errors);

    void print(std::ostream &stream) const override;

    bool emplaceRules(std::map<std::string, Rule *> &rules,
           ParseBuffer::Error &errors) override;

    virtual ~Reference();

};

}; // Namespace Pimlico
