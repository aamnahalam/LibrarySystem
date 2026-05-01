#ifndef BORROWRECORD_H
#define BORROWRECORD_H

#include <string>
#include <ctime>
using namespace std;

class User; // forward declaration

class BorrowRecord {
private:
    User* user;
    string resourceName;
    string borrowDate;
    string dueDate;
    string returnDate;
    bool isReturned;

public:
    BorrowRecord(User* user, string resourceName, string borrowDate, string dueDate);

    void markAsReturned(string returnDate);
    bool getReturnStatus();

    string getResourceName() const;
    string getBorrowDate() const;
    string getDueDate() const;

    void showRecord();

    // Additional methods needed by Admin
    bool isOverdue() const;
    double calculateFine() const;
};

#endif