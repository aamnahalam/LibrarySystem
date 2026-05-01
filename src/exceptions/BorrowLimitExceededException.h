#ifndef BORROW_LIMIT_EXCEEDED_EXCEPTION_H
#define BORROW_LIMIT_EXCEEDED_EXCEPTION_H

#include "LibraryException.h"
#include <string>
using namespace std;

class BorrowLimitExceededException : public LibraryException {
public:
    BorrowLimitExceededException(string message);
};

#endif