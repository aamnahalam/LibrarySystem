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

static bool parseDateString(const string &date, int &year, int &month, int &day)
{
    if (date.size() != 10 || date[4] != '-' || date[7] != '-')
        return false;
    try {
        year = stoi(date.substr(0, 4));
        month = stoi(date.substr(5, 2));
        day = stoi(date.substr(8, 2));
    } catch (const exception &) {
        return false;
    }
    if (month < 1 || month > 12 || day < 1 || day > 31)
        return false;
    return true;
}

void BorrowRecord::showRecord() {
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

double BorrowRecord::calculateFine() const
{
    if (isReturned)
        return 0.0;

    int dueYear, dueMonth, dueDay;
    if (!parseDateString(dueDate, dueYear, dueMonth, dueDay))
        return 0.0;

    // Get current date
    time_t now = time(0);
    tm *currentTime = localtime(&now);
    int currentYear = 1900 + currentTime->tm_year;
    int currentMonth = 1 + currentTime->tm_mon;
    int currentDay = currentTime->tm_mday;

    // Calculate days overdue
    int daysOverdue = 0;
    if (currentYear > dueYear)
    {
        daysOverdue = (currentYear - dueYear) * 365;
    }
    if (currentMonth > dueMonth)
    {
        daysOverdue += (currentMonth - dueMonth) * 30;
    }
    if (currentDay > dueDay)
    {
        daysOverdue += (currentDay - dueDay);
    }

    // Fine rate: $1 per day
    return daysOverdue > 0 ? daysOverdue * 1.0 : 0.0;
}