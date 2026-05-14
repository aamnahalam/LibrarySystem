#ifndef RESOURCENOTAVAILABLEEXCEPTION_H
#define RESOURCENOTAVAILABLEEXCEPTION_H

#include "LibraryException.h"
#include <string>
using namespace std;

class ResourceNotAvailableException : public LibraryException {
public:
    ResourceNotAvailableException(string message) : LibraryException(message) {}
    virtual ~ResourceNotAvailableException() noexcept = default;
};
#endif
