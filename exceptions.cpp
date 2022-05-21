#include "exceptions.h"

const char *MatamProjectException::what() const noexcept
{
    return result.c_str();
}
