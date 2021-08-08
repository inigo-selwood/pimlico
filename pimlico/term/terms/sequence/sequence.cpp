#include "sequence.hpp"

#include "../choice/choice.hpp"

Sequence::Sequence() : Term(Term::Type::SEQUENCE) {}

Term *Sequence::parse(ParseBuffer &buffer, ErrorBuffer &errors) {
    Sequence *sequence = new Sequence();
    sequence->position = buffer.position;

    while(true) {

        Term *value = Choice::parse(buffer, errors);
        if(value == nullptr)
            return nullptr;
        sequence->values.push_back(value);

        buffer.skip(true);
        if(buffer.finished() || buffer.peek('\n') || buffer.peek(')'))
            break;
    }

    if(sequence->values.size() == 1)
        return sequence->values.back();

    return sequence;
}