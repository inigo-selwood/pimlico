#pragma once

#include <pimlico.hpp>

namespace Pimlico {

class Sequence : public Term {

public:

    std::vector<Term *> values;

    Sequence();

    static Term *parse(ParseBuffer &buffer, ParseBuffer::Error &errors);

    void print(std::ostream &stream) const override;

    bool emplaceRules(std::map<std::string, Rule *> &rules,
           ParseBuffer::Error &errors) override;

    virtual ~Sequence();

};

}; // Namespace Pimlico
