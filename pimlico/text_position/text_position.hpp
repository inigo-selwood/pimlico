#pragma once

#include <utility>
#include <vector>

struct TextPosition {

public:

    long index;

    long line;
    int column;

    unsigned int block_indentation;

    bool line_broken;

    TextPosition(const std::vector<std::pair<long, int>> &line_indices);
    TextPosition();

};
