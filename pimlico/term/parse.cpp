#include <tuple>

#include "term.hpp"

#include "terms/choice/choice.hpp"
#include "terms/sequence/sequence.hpp"
#include "terms/constant/constant.hpp"
#include "terms/reference/reference.hpp"
#include "terms/range/range.hpp"

#include "../parse_buffer/parse_buffer.hpp"
#include "../error_buffer/error_buffer.hpp"

/* Parse a term

Arguments
---------
buffer
    the buffer from which to parse the term
errors
    buffer for error managing
root
    whether the term is the root of a block

Returns
-------
term
    the parsed term, or nullptr if an error was encountered
*/
Term *Term::parse(ParseBuffer &buffer,
        ErrorBuffer &errors,
        const bool root = false) {

    // Treat the root term as a sequence
    if(root)
        return Sequence::parse(buffer, errors);

    Term *term = nullptr;

    // Check for predicates
    term->predicate = Predicate::NONE;
    if(buffer.read('&'))
        term->predicate = Predicate::AND;
    else if(buffer.read('!'))
        term->predicate = Predicate::NOT;

    // Check if the term is enclosed
    buffer.skip();
    bool enclosed = buffer.read('(');
    if(enclosed)
        term = Sequence::parse(buffer, errors);

    // Delegate parsing
    else {
        char character = buffer.peek();

        if(character == '\'')
            term = Constant::parse(buffer, errors);

        else if(character == '[')
            term = Range::parse(buffer, errors);

        else if((character >= 'a' && character <= 'z') || character == '_')
            term = Reference::parse(buffer, errors);

        else {
            errors.add("expected a term", buffer.position);
            return nullptr;
        }
    }

    // Check parsing succeeded
    if(term == nullptr)
        return nullptr;

    // Ensure enclosed terms are closed
    buffer.skip();
    if(enclosed && buffer.read(')') == false) {
        errors.add("expected ')'", buffer.position);
        return nullptr;
    }

    // Parse bounds
    buffer.skip();
    term->bounds = parse_bounds(buffer, errors);
    if(std::get<0>(term->bounds) == 0 && std::get<1>(term->bounds) == 0)
        return nullptr;

    return term;
}
