#include "BorrowRecord.h"
#include <iostream>
using namespace std;

BorrowRecord::BorrowRecord(User* user, string resourceName, string borrowDate) {
    this->user = user;
    this->resourceName = resourceName;
    this->borrowDate = borrowDate;
}

void BorrowRecord::showRecord() {
    cout << "User: ";
    user->displayInfo();
    cout << "Book: " << resourceName << endl;
    cout << "Date: " << borrowDate << endl;
}