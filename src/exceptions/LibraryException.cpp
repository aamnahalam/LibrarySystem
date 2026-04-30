#include "LibraryException.h"

LibraryException::LibraryException(string message) {
    this->message = message;
}

string LibraryException::getMessage() {
    return message;
}