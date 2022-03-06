#include "../parse_buffer.hpp"

namespace Pimlico {

// Constructor
ParseBuffer::Position::Position() {
    this->index = 0;
    this->line = 0;
    this->column = 0;
}

// Constructor
ParseBuffer::Position::Position(const bool &empty) {
    this->index = 0;

    // Initialize the line, column values to zero if the text the position is
    // referencing is empty, or 1 otherwise
    const int start_index = empty == 0;
    this->line = start_index;
    this->column = start_index;
}

}; // Namespace Pimlico
