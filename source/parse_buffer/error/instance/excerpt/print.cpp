#include "excerpt.hpp"

namespace Pimlico {

void Excerpt::print(std::ostream &stream) const {
    if(this->text.empty() == false) {
        stream << this->position
                << " (" << this->source << ") "
                << this->message << '\n';
        stream << "    " << this->text;
    }
    else
        stream << "(" << this->source << ") " << this->message;
}

}; // Namespace Pimlico
