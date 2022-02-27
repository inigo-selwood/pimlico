#include "form.hpp"

namespace Pimlico {

namespace Buffer {

std::string Form::serialize() const {
    std::string result;

    long index = 0;
    for(const Form::Entry &entry : this->entries) {

        // Do nothing if the entry's token has no entry in the substitutions
        // table
        if(this->substitutions.find(entry.token) == this->substitutions.end())
            continue;

        // Add the substring from the current position to the next entry
        const long delta = entry.index - index;
        if(delta != 0) {
            const std::string substring = this->text.substr(index, delta);
            result += substring;
            index += substring.length();
        }

        // Insert the token
        result += this->substitutions.at(entry.token);
        index += entry.token.length() + 1;
    }

    const long delta = this->text.length() - index;
    if(delta > 0)
        result += this->text.substr(index, std::string::npos);

    return result;
}

};

};
