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

string BorrowRecord::getResourceName() const {
    return resourceName;
}

string BorrowRecord::getBorrowDate() const {
    return borrowDate;
}

string BorrowRecord::getDueDate() const {
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

bool BorrowRecord::isOverdue() const {
    if (isReturned) return false;
    
    // Parse due date (format: YYYY-MM-DD)
    int dueYear = stoi(dueDate.substr(0, 4));
    int dueMonth = stoi(dueDate.substr(5, 2));
    int dueDay = stoi(dueDate.substr(8, 2));
    
    // Get current date
    time_t now = time(0);
    tm* currentTime = localtime(&now);
    int currentYear = 1900 + currentTime->tm_year;
    int currentMonth = 1 + currentTime->tm_mon;
    int currentDay = currentTime->tm_mday;
    
    // Compare dates
    if (currentYear > dueYear) return true;
    if (currentYear < dueYear) return false;
    if (currentMonth > dueMonth) return true;
    if (currentMonth < dueMonth) return false;
    return currentDay > dueDay;
}

double BorrowRecord::calculateFine() const {
    if (isReturned) return 0.0;
    
    // Parse due date
    int dueYear = stoi(dueDate.substr(0, 4));
    int dueMonth = stoi(dueDate.substr(5, 2));
    int dueDay = stoi(dueDate.substr(8, 2));
    
    // Get current date
    time_t now = time(0);
    tm* currentTime = localtime(&now);
    int currentYear = 1900 + currentTime->tm_year;
    int currentMonth = 1 + currentTime->tm_mon;
    int currentDay = currentTime->tm_mday;
    
    // Calculate days overdue
    int daysOverdue = 0;
    if (currentYear > dueYear) {
        daysOverdue = (currentYear - dueYear) * 365;
    }
    if (currentMonth > dueMonth) {
        daysOverdue += (currentMonth - dueMonth) * 30;
    }
    if (currentDay > dueDay) {
        daysOverdue += (currentDay - dueDay);
    }
    
    // Fine rate: $1 per day
    return daysOverdue > 0 ? daysOverdue * 1.0 : 0.0;
}