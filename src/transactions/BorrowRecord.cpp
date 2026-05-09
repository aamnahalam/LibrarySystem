#include "BorrowRecord.h"
#include "../users/user.h"
#include <iostream>
using namespace std;

BorrowRecord::BorrowRecord(User *user, int resourceID, string resourceName, string borrowDate, string dueDate)
{
    this->user = user;
    this->resourceID = resourceID;
    this->resourceName = resourceName;
    this->borrowDate = borrowDate;
    this->dueDate = dueDate;
    this->isReturned = false;
}

void BorrowRecord::markAsReturned(string returnDate)
{
    this->returnDate = returnDate;
    this->isReturned = true;
}

bool BorrowRecord::getReturnStatus() const
{
    return isReturned;
}

int BorrowRecord::getResourceID() const
{
    return resourceID;
}

string BorrowRecord::getResourceName() const
{
    return resourceName;
}

string BorrowRecord::getBorrowDate() const
{
    return borrowDate;
}

string BorrowRecord::getDueDate() const
{
    return dueDate;
}

string BorrowRecord::getReturnDate() const
{
    return returnDate;
}

void BorrowRecord::showRecord() const {
    cout << "User: " << user->getFullName() << " (ID: " << user->getID() << ")" << endl;
    cout << "Email: " << user->getEmail() << endl;
    cout << "Resource: " << resourceName << endl;
    cout << "Borrow Date: " << borrowDate << endl;
    cout << "Due Date: " << dueDate << endl;

    if (isReturned)
    {
        cout << "Returned on: " << returnDate << endl;
    }
    else
    {
        cout << "Status: Not Returned" << endl;
    }
}

bool BorrowRecord::isOverdue() const
{
    if (isReturned)
        return false;

    int dueYear, dueMonth, dueDay;
    if (!parseDateString(dueDate, dueYear, dueMonth, dueDay))
        return false;

    // Get current date
    time_t now = time(0);
    tm *currentTime = localtime(&now);
    int currentYear = 1900 + currentTime->tm_year;
    int currentMonth = 1 + currentTime->tm_mon;
    int currentDay = currentTime->tm_mday;

    // Compare dates
    if (currentYear > dueYear)
        return true;
    if (currentYear < dueYear)
        return false;
    if (currentMonth > dueMonth)
        return true;
    if (currentMonth < dueMonth)
        return false;
    return currentDay > dueDay;
}

double BorrowRecord::calculateFine(double fineRate, double discountMultiplier, const string& actualReturnDate) const
{
    string compareDate;
    if (!actualReturnDate.empty()) {
        compareDate = actualReturnDate;
    } else if (isReturned) {
        compareDate = returnDate;
    } else {
        time_t now = time(0);
        tm *currentTime = localtime(&now);
        int currentYear = 1900 + currentTime->tm_year;
        int currentMonth = 1 + currentTime->tm_mon;
        int currentDay = currentTime->tm_mday;

        compareDate = to_string(currentYear) + "-" +
                      (currentMonth < 10 ? "0" : "") + to_string(currentMonth) + "-" +
                      (currentDay < 10 ? "0" : "") + to_string(currentDay);
    }

    int dueYear, dueMonth, dueDay;
    if (!parseDateString(dueDate, dueYear, dueMonth, dueDay))
        return 0.0;

    // Parse comparison date
    int compYear = stoi(compareDate.substr(0, 4));
    int compMonth = stoi(compareDate.substr(5, 2));
    int compDay = stoi(compareDate.substr(8, 2));

    // Calculate days overdue
    int daysOverdue = 0;
    if (compYear > dueYear)
    {
        daysOverdue = (compYear - dueYear) * 365;
    }
    if (compMonth > dueMonth)
    {
        daysOverdue += (compMonth - dueMonth) * 30;
    }
    if (compDay > dueDay)
    {
        daysOverdue += (compDay - dueDay);
    }

    double fine = daysOverdue > 0 ? daysOverdue * fineRate : 0.0;
    fine *= discountMultiplier;
    return fine;
}

bool BorrowRecord::parseDateString(const string& date, int& year, int& month, int& day) const {
    try {
        // Expected format: YYYY-MM-DD
        if (date.length() != 10 || date[4] != '-' || date[7] != '-') {
            return false;
        }
        year = stoi(date.substr(0, 4));
        month = stoi(date.substr(5, 2));
        day = stoi(date.substr(8, 2));
        return true;
    } catch (...) {
        return false;
    }
}