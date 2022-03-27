#include "print_engine.hpp"

namespace Pimlico {

inline std::string readToken(ParseBuffer &buffer) {
    std::string result;

    while(true) {
        if(buffer.finished())
            break;

        const char character = buffer.peek();
        if((character >= 'a' && character <= 'z')
                || (character >= 'A' && character <= 'Z')
                || (character >= '0' && character <= '9')
                || character == '_')
            result += buffer.read();
        else
            break;
    }

    return result;
}

std::string PrintEngine::render() {
    std::string result;

    ParseBuffer buffer(this->text);

    while(true) {
        if(buffer.finished())
            break;

        const char character = buffer.read();
        if(character != '@') {
            result += character;
            continue;
        }

        const std::string token = readToken(buffer);
        if(this->substitutions.find(token) == this->substitutions.end()) {
            result += "@";
            result += token;
            continue;
        }

        result += this->substitutions[token];
    }

    return result;
}

};
