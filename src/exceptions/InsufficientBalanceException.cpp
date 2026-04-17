#include "InsufficientBalanceException.h"

InsufficientBalanceException::InsufficientBalanceException(string message)
    : LibraryException(message) {
}
