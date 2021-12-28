#include "term.hpp"

#include "terms/choice/choice.hpp"
#include "terms/constant/constant.hpp"
#include "terms/range/range.hpp"
#include "terms/reference/reference.hpp"
#include "terms/sequence/sequence.hpp"

namespace Pimlico {

/* Parse an instance bound
Arguments
---------
buffer
    the buffer from which to parse the bound
errors
    buffer for error reporting
Returns
-------
bound
    the instance bound, or -1 if no bound was found
*/
static int parse_bound_value(Buffer::Parse &buffer, Buffer::Error &errors) {

    // Extract digits
    std::string text;
    while(true) {
        if(buffer.finished())
            break;

        const char character = buffer.peek();
        if(character >= '0' && character <= '9')
            text += buffer.read();
        else
            break;
    }

    // Check digits were found
    if(text.empty())
        return -1;

    // Parse and return integer
    return std::stoi(text);
}

static std::string parse_binding(Buffer::Parse &buffer) {
    std::string result;
    while(true) {
        if(buffer.finished())
            break;

        const char character = buffer.peek();
        if(character >= 'a' && character <= 'z' || character == '_')
            result += buffer.read();
        else
            break;
    }

    return result;
}

/* Parse a specific bound
Specific bounds, unlike single-character type-hints, give the writer more
flexibility in setting range counts. They can be formatted in the following
ways:
```
term{n}     # N instances
term{:n}    # Up to N instances
term{n:}    # N or more instances
term{n:m}   # Between N and M instances
```
Arguments
---------
buffer
    the buffer from which to parse the bound
errors
    buffer for error reporting
Returns
-------
bound
    the instance bound, or {0, 0} if the bound was badly formatted. `errors`
    will have been set as appropriate
*/
static Term::Bounds parse_specific_bounds(Buffer::Parse &buffer,
        Buffer::Error &errors) {

    if(buffer.read('{') == false)
        throw "expected '{'";

    // Parse the range-start value
    int start_value = parse_bound_value(buffer, errors);

    // Check to see if a colon is present
    buffer.skip_space();
    bool colon_present = false;
    if(buffer.read(':'))
        colon_present = true;

    // Parse the range-end value
    buffer.skip_space();

    int end_value = parse_bound_value(buffer, errors);

    if(buffer.read('}') == false) {
        errors.add("expected '}'", buffer);
        return {0, 0};
    }

    // N instances
    if(start_value != -1 && end_value == -1 && colon_present == false) {
        if(start_value == 0) {
            errors.add("zero-valued instance bound", buffer);
            return {0, 0};
        }

        return {start_value, start_value};
    }

    // N or more instances
    else if(start_value != -1 && end_value == -1 && colon_present)
        return {start_value, -1};

    // Up to N instances
    else if(start_value == -1 && end_value != -1 && colon_present) {
        if(end_value == 0) {
            errors.add("zero-valued upper instance bound", buffer);
            return {0, 0};
        }

        return {-1, end_value};
    }

    // Between N and M values
    else if(start_value != -1 && end_value != -1 && colon_present) {
        if(end_value < start_value) {
            errors.add("illogical instance bound", buffer);
            return {0, 0};
        }
        else if(start_value == end_value && start_value == 0) {
            errors.add("zero-instance bound", buffer);
            return {0, 0};
        }

        return {start_value, end_value};
    }

    // Report an error if the bound was invalid
    else {
        errors.add("invalid instance bound", buffer);
        return {0, 0};
    }
}

/* Parse instance bounds (if present)
Instance bounds come in several forms:
```
# Single-character hints
term+   # One-or-more instances
term*   # Zero-or-more instances
term?   # An optional instance
# Specific intance bounds
term{n}     # N instances
term{:n}    # Up to N instances
term{n:}    # N or more instances
term{n:m}   # Between N and M instances
```
Arguments
---------
buffer
    the buffer from which to parse the bound
errors
    buffer for error reporting
Returns
-------
bound
    the instance bounds, or {0, 0} if an error was encountered. Defaults to
    {1, 1} if no bounds were present
*/
static Term::Bounds parse_bounds(Buffer::Parse &buffer, Buffer::Error &errors) {

    // Handle single-character instance-hint values
    if(buffer.read('?'))
        return {0, 1};
    else if(buffer.read('*'))
        return {0, -1};
    else if(buffer.read('+'))
        return {1, -1};

    // Parse specific bounds
    else if(buffer.peek('{')) {

        // Copy the buffer's position to reset it later
        const Buffer::Position start_position = buffer.position;

        // Check if the bracket is followed by a newline -- that's almost
        // certainly an embedded expression
        buffer.read('{');
        buffer.skip_space();
        if(buffer.peek('\n')) {
            buffer.position = start_position;
            return {1, 1};
        }

        // Here we're trying to guess if the curly bracket is the start of a
        // bounds specifier, or an embedded expression.
        // In the former case, we'd see one or more digits, a semi-colon, and
        // a closing bracket. We can even be a bit tolerant about having all
        // three, to avoid misinterpreting typos

        // This loop counts how many of those things are encountered in the
        // next 12 characters of the buffer, and makes a weighted guess about
        // what it needs to parse next.

        // If it's an embedded expression, we just return the default bounds,
        // and allow the production which invoked this function to handle
        // whatever the embedded slice is.
        bool digits = false;
        bool semi_colon = false;
        bool terminated = false;
        bool newline = false;
        bool tabs = false;
        bool colon = false;
        for(int index = 0; index < 12; index += 1) {
            if(buffer.finished()) {
                errors.add("expected '}'", buffer);
                return {0, 0};
            }

            // Check whether the character at the current index is a digit,
            // a semi-colon, or the closing bracket
            const char character = buffer.peek();
            if(character >= '0' && character <= '9')
                digits = true;
            else if(character == ':')
                semi_colon = true;
            else if(character == '\t')
                tabs = true;
            else if(character == ';')
                colon = true;
            else if(buffer.peek('}')) {
                terminated = true;
                break;
            }

            // Increment the buffer
            buffer.read();
        }

        // Reset the buffer's position for whatever comes next
        buffer.position = start_position;

        // Calculate some heuristic of the likelihood of this being a range
        // value, as a weighted sum of the different characteristics of both
        // bounds values and embedded expressions
        const int probability = digits
                + (semi_colon * 2)
                + terminated
                - colon
                - tabs
                - (newline * 2);

        // If there's any combination of two or more of the criteria, it's
        // likely to be a specific bound value
        if(probability >= 2)
            return parse_specific_bounds(buffer, errors);

        return {1, 1};
    }

    // Otherwise, default to one instance
    else
        return {1, 1};
}

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
Term *Term::parse(Buffer::Parse &buffer,
        Buffer::Error &errors,
        const bool root) {

    // Check if there's a binding hint
    std::string binding;
    const char character = buffer.peek();
    if((character >= 'a' && character <= 'z') || character == '_') {
        Buffer::Position start_position = buffer.position;
        const std::string identifier = parse_binding(buffer);

        buffer.skip_space();
        if(identifier.empty() == false && buffer.read(':'))
            binding = identifier;
        else
            buffer.position = start_position;
    }

    // Treat the root term as a sequence
    if(root) {
        Term *sequence = Sequence::parse(buffer, errors);
        sequence->binding = binding;
        return sequence;
    }

    // Check for predicates
    Term::Predicate predicate = Predicate::NONE;
    if(buffer.read('&'))
        predicate = Predicate::AND;
    else if(buffer.read('!'))
        predicate = Predicate::NOT;

    // Check if the term is enclosed
    buffer.skip_space();
    Term *term = nullptr;
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
            errors.add("expected a term", buffer);
            return nullptr;
        }
    }

    // Check parsing succeeded
    if(term == nullptr)
        return nullptr;

    // Ensure enclosed terms are closed
    buffer.skip_space();
    if(enclosed && buffer.read(')') == false) {
        errors.add("expected ')'", buffer);
        delete term;
        return nullptr;
    }

    // Parse bounds
    buffer.skip_space();
    term->bounds = parse_bounds(buffer, errors);
    if(std::get<0>(term->bounds) == 0 && std::get<1>(term->bounds) == 0) {
        delete term;
        return nullptr;
    }

    term->predicate = predicate;
    term->binding = binding;
    return term;
}

}; // Namespace Pimlico
