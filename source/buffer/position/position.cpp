#include "position.hpp"

namespace Pimlico {

namespace Buffer {

// Constructor
Position::Position() {
    this->index = 0;
    this->line = 0;
    this->column = 0;
}

// Constructor
Position::Position(const bool &empty) {
    this->index = 0;

    // Initialize the line, column values to zero if the text the position is
    // referencing is empty, or 1 otherwise
    const int start_index = empty == 0;
    this->line = start_index;
    this->column = start_index;
}

}; // Namespace Buffer

}; // Namespace Pimlico
