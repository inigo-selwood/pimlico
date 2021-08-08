#include <array>

#include "term.hpp"

Term::Term(const Term::Type &type) {
    this->bounds = std::array<int, 2>({1, 1});
    this->type = type;
    this->predicate = Predicate::NONE;
}