#include "position.hpp"

namespace Pimlico {

namespace Buffer {

bool Position::operator==(const Position &other) const {
    return (this->index == other.index)
            && (this->line == other.line)
            && (this->column == other.column);
}

}; // Namespace Buffer

}; // Namespace Pimlico
