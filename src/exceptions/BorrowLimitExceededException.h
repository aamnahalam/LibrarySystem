#ifndef BORROWLIMITEXCEEDEDEXCEPTION_H
#define BORROWLIMITEXCEEDEDEXCEPTION_H

#include "LibraryException.h"

class BorrowLimitExceededException : public LibraryException {
public:
    BorrowLimitExceededException(string message);
};

#endif