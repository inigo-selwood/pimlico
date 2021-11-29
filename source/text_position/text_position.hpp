#pragma once

#include <ostream>
#include <utility>
#include <vector>

struct TextPosition {

public:

    long index;

    long line;
    int column;

    unsigned int block_indentation;

    bool line_broken;

    friend std::ostream &operator<<(std::ostream &stream, 
            const TextPosition &position);

    TextPosition();
    TextPosition(const std::vector<std::pair<long, int>> &line_indices);
    
    void print(std::ostream &stream) const;

};
