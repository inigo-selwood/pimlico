#pragma once

#include <string>

#include "../parse/parse.hpp"
#include "../position/position.hpp"

namespace Pimlico {

namespace Buffer {

class Error {

public:

    class Instance  {

    public:

        virtual void print(std::ostream &stream) const = 0;

        virtual ~Instance() = default;

    };

    friend std::ostream &operator<<(std::ostream &stream,
            const Error &position);

    operator bool() const;

    void add(const std::string &source, const std::string &text);
    void add(const std::string &source,
            const std::string &text,
            const Parse &buffer);
    void add(const std::string &source,
            const std::string &text,
            const Parse &buffer,
            const Position &position);

    void print(std::ostream &stream) const;

    ~Error();

private:

    std::vector<Instance *> instances;

};

}; // Namespace Buffer

}; // Namespace Pimlico
