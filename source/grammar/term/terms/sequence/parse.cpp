#include "sequence.hpp"

#include "../choice/choice.hpp"
#include "../constant/constant.hpp"
#include "../range/range.hpp"
#include "../reference/reference.hpp"

namespace Pimlico {

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

    while(true) {

        Term *value = Choice::parse(buffer, errors);
        if(value == nullptr)
            return nullptr;
        sequence->values.push_back(value);

        buffer.skip_space();
        if(buffer.finished()
                || buffer.peek('{')
                || buffer.peek('\n')
                || buffer.peek(')'))
            break;
    }

    if(sequence->values.size() == 1)
        return sequence->values.back();

    return sequence;
}

}; // Namespace Pimlico
