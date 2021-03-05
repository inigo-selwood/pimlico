#pragma once

#include "term.hpp"

class Rule : public Term {

public:

    static SharedPointer parse(TextBuffer &buffer,
            Error::Vector &errors);

};
