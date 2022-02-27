#pragma once

#include <map>
#include <string>
#include <vector>

namespace Pimlico {

namespace Buffer {

class Form {

public:

    struct Entry {

    public:

        std::string token;

        long index;

    };

    void build(const std::string &text);

    void substitute(const std::string &token, const std::string &value);

    std::string serialize() const;

private:

    std::vector<Entry> entries;

    std::map<std::string, std::string> substitutions;

    std::string text;

};

};

};
