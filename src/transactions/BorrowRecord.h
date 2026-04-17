#ifndef BORROWRECORD_H
#define BORROWRECORD_H

#include <string>
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

    string getResourceName();
    string getBorrowDate();
    string getDueDate();

    void showRecord();
};

#endif