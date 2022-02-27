#include "form.hpp"

#include <pimlico.hpp>

namespace Pimlico {

namespace Buffer {

void Form::build(const std::string &text) {
    this->text = text;

    Parse buffer(text);
    while(buffer.finished() == false) {
        if(buffer.read() != '@')
            continue;

        std::string token;
        const long index = buffer.position.index - 1;
        while(true) {
            if(buffer.finished())
                break;

            const char character = buffer.read();
            if((character < 'a' || character > 'z')
                    && (character < 'A' || character > 'Z')
                    && character != '_')
                break;

            token += character;
        }

        if(token.empty())
            continue;

        Form::Entry entry;
        entry.token = token;
        entry.index = index;

        this->entries.push_back(entry);
    }
}

};

};
