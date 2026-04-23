#include "BorrowLimitExceededException.h"

BorrowLimitExceededException::BorrowLimitExceededException(string message)
    : LibraryException(message) {
}