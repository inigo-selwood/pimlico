#pragma once

#include <ostream>

namespace Pimlico {

namespace Buffer {

class Instance  {

public:

    friend std::ostream &operator<<(std::ostream &stream,
            const Instance &message);

    virtual void print(std::ostream &stream) const = 0;

    virtual ~Instance() = default;

};

}; // Namespace Buffer

}; // Namespace Pimlico
