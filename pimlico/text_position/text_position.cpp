#include <utility>
#include <vector>

#include "text_position.hpp"

TextPosition::TextPosition() {
    this->index = 0;

    this->line = 0;
    this->column = 0;

    this->block_indentation = 0;
    this->line_broken = false;
}

TextPosition::TextPosition(
        const std::vector<std::pair<long, int>> &line_indices) {

    this->index = 0;

    this->line = 0;
    this->column = 0;

    this->block_indentation = 0;
    this->line_broken = false;

    // If there are lines available, calculate the block indentation and line
    // broken flag
    int line_count = line_indices.size();
    if(line_count) {
        int indentation = line_indices.front().second;
        int indentation_target = this->block_indentation + 8;

        this->block_indentation = line_indices.front().second;
        this->line_broken = line_count && (indentation >= indentation_target);
    }

    this->line_broken = false;
}
