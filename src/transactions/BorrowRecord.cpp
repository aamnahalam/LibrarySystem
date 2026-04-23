#include "BorrowRecord.h"
#include "user.h"
#include <iostream>
using namespace std;

BorrowRecord::BorrowRecord(User* user, string resourceName, string borrowDate, string dueDate) {
    this->user = user;
    this->resourceName = resourceName;
    this->borrowDate = borrowDate;
    this->dueDate = dueDate;
    this->isReturned = false;
}

void BorrowRecord::markAsReturned(string returnDate) {
    this->returnDate = returnDate;
    this->isReturned = true;
}

bool BorrowRecord::getReturnStatus() {
    return isReturned;
}

string BorrowRecord::getResourceName() {
    return resourceName;
}

string BorrowRecord::getBorrowDate() {
    return borrowDate;
}

string BorrowRecord::getDueDate() {
    return dueDate;
}

void BorrowRecord::showRecord() {
    cout << "User: ";
    user->displayInfo();

    cout << "Resource: " << resourceName << endl;
    cout << "Borrow Date: " << borrowDate << endl;
    cout << "Due Date: " << dueDate << endl;

    if (isReturned) {
        cout << "Returned on: " << returnDate << endl;
    } else {
        cout << "Status: Not Returned" << endl;
    }
}