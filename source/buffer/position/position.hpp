#pragma once

#include <ostream>

namespace Pimlico {

namespace Buffer {

struct Position {

public:

    long index;

    long line;
    int column;

    friend std::ostream &operator<<(std::ostream &stream,
            const Position &position);

    bool operator==(const Position &other) const;

    Position();
    Position(const bool &empty);

    void print(std::ostream &stream) const;

};

}; // Namespace Buffer

}; // Namespace Pimlico
