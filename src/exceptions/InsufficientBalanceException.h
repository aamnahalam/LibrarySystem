#ifndef INSUFFICIENTBALANCEEXCEPTION_H
#define INSUFFICIENTBALANCEEXCEPTION_H

#include "LibraryException.h"
#include <string>
using namespace std;

class InsufficientBalanceException : public LibraryException {
public:
    InsufficientBalanceException(string message) : LibraryException(message) {}
    virtual ~InsufficientBalanceException() noexcept = default;
};

#endif