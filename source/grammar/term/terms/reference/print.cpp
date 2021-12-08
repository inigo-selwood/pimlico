#include "reference.hpp"

namespace Pimlico {
        
/* Print a term instance

Arguments
---------
stream
    the stream to print to
*/
void Reference::print(std::ostream &stream) const {
    stream << this->name;
}

}; // Namespace Pimlico
