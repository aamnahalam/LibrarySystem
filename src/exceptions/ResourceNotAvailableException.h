#ifndef RESOURCENOTAVAILABLEEXCEPTION_H
#define RESOURCENOTAVAILABLEEXCEPTION_H

#include "LibraryException.h"

class ResourceNotAvailableException : public LibraryException {
public:
    ResourceNotAvailableException(string message);
};

#endif
