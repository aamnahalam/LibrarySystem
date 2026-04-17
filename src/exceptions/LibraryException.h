#ifndef LIBRARYEXCEPTION_H
#define LIBRARYEXCEPTION_H

#include <string>
using namespace std;

class LibraryException {
protected:
    string message;

public:
    LibraryException(string message);
    string getMessage();
};

#endif
