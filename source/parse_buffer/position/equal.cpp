#include "../parse_buffer.hpp"

namespace Pimlico {

bool ParseBuffer::Position::operator==(
        const ParseBuffer::Position &other) const {

    return (this->index == other.index)
            && (this->line == other.line)
            && (this->column == other.column);
}

}; // Namespace Pimlico
