#include "sequence.hpp"

#include "../choice/choice.hpp"
#include "../constant/constant.hpp"
#include "../range/range.hpp"
#include "../reference/reference.hpp"

namespace Pimlico {

static void delete_values(std::vector<Term *> &values) {
    while(values.empty() == false) {
        if(values.back())
            delete values.back();

        values.pop_back();
    }
}

/* Parses a Sequence term

Sequences have the following format:

`term_1 term_2 term_3`

And so on...

Arguments
---------
buffer
    the buffer to parse sequence from
errors
    buffer for error reporting

Returns
-------
term
    a populated Sequence instance, or nullptr if an error is encountered
*/
Term *Sequence::parse(Buffer::Parse &buffer, Buffer::Error &errors) {
    Sequence *sequence = new Sequence();
    sequence->position = buffer.position;

    std::vector<Term *> values;
    while(true) {

        Term *value = Choice::parse(buffer, errors);
        if(value == nullptr) {
            delete_values(values);
            delete sequence;
            return nullptr;
        }
        values.push_back(value);

        buffer.skip_space();
        if(buffer.finished()
                || buffer.peek('{')
                || buffer.peek('\n')
                || buffer.peek(')'))
            break;
    }

    if(values.size() == 1) {
        delete sequence;
        return values.back();
    }

    sequence->values = values;
    return sequence;
}

}; // Namespace Pimlico
