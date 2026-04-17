#ifndef INSUFFICIENTBALANCEEXCEPTION_H
#define INSUFFICIENTBALANCEEXCEPTION_H

#include "LibraryException.h"

class InsufficientBalanceException : public LibraryException {
public:
    InsufficientBalanceException(string message);
};

#endif