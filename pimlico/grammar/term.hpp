#pragma once

class Term {

public:

    enum class Type {
        NONE = 0,

        CONSTANT,
        RANGE,
        REFERENCE,

        CHOICE,
        SEQUENCE,
    };

    enum class Predicate {
        NONE = 0,

        AND,
        NOT,
    };

    typedef std::array<int, 2> Bounds;

    std::variant<std::vector<Term>, std::string, std::array<char, 2>> value;

    Bounds bounds;

    Predicate predicate;

    Type type;

    friend std::ostream &operator<<(std::ostream &stream, const Term &term) {
        if(term.predicate == Term::Predicate::AND)
            stream << '&';
        else if(term.predicate == Term::Predicate::NOT)
            stream << '!';

        if(term.type == Term::Type::CONSTANT) {
            const auto &value = std::get<std::string>(term.value);
            stream << "\'" << value << "\'";
        }

        else if(term.type == Term::Type::RANGE) {
            const auto &value = std::get<std::array<char, 2>>(term.value);
            stream << "[" << value[0] << "-" << value[1] << "]";
        }

        else if(term.type == Term::Type::REFERENCE)
            stream << std::get<std::string>(term.value);

        else if(term.type == Term::Type::CHOICE) {
            const auto &values = std::get<std::vector<Term>>(term.value);
            const int value_count = values.size();
            for(int index = 0; index < value_count; index += 1) {
                stream << values[index];
                if(index + 1 < value_count)
                    stream << " | ";
            }
        }

        else if(term.type == Term::Type::SEQUENCE) {
            for(const auto &value : std::get<std::vector<Term>>(term.value))
                stream << value << " ";
        }

        else
            stream << "null";

        // The default bounds case, checked first
        if(term.bounds == Bounds({1, 1}))
            return stream;

        // Special signage bounds cases
        else if(term.bounds == Bounds({0, 1}))
            stream << '?';
        else if(term.bounds == Bounds({0, -1}))
            stream << '*';
        else if(term.bounds == Bounds({1, -1}))
            stream << '+';

        // Specific bounds
        else if(term.bounds[0] == term.bounds[1])
            stream << '{' << term.bounds[0] << '}';
        else if(term.bounds[0] == -1 && term.bounds[1] != -1)
            stream << "{:" << term.bounds[1] << "}";
        else if(term.bounds[0] != -1 && term.bounds[1] == -1)
            stream << '{' << term.bounds[0] << ":}";
        else {
            stream << '{' << term.bounds[0] << ':' << term.bounds[1] << '}'; 
        }

        return stream;
    }

    Term() : bounds({1, 1}), predicate(Predicate::NONE), type(Type::NONE) {}

};
