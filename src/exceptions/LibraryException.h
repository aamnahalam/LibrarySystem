#ifndef LIBRARYEXCEPTION_H
#define LIBRARYEXCEPTION_H

#include <string>
#include <exception>
using namespace std;

class LibraryException : public exception
{
protected:
    string message;

public:
    LibraryException(string msg) : message(msg) {}
    string getMessage() const
    {
        return message;
    }
    const char *what() const noexcept override
    {
        return message.c_str();
    }
};

#endif