#include <string>

#include "../text_position/text_position.hpp"
#include "../term/term.hpp"

#include "../error_buffer/error_buffer.hpp"
#include "../parse_buffer/parse_buffer.hpp"

class Rule {

public:

    TextPosition position;

    Term *value;

    std::string name;

    Rule();

    static Rule *parse(ParseBuffer &buffer, ErrorBuffer &errors);

};
