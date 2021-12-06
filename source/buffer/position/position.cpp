#include "position.hpp"

namespace Pimlico {

namespace Buffer {

Position::Position() {
    this->index = 0;

    this->line = 1;
    this->column = 1;
}

}; // Namespace Buffer

}; // Namespace Pimlico
