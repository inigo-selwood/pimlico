#pragma once

#include <ostream>
#include <string>

#include "../../parse_buffer/parse_buffer.hpp"
#include "../../text_position/text_position.hpp"

struct Error {

public:

    virtual void print(std::ostream &stream) const;

};
