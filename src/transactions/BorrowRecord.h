#ifndef BORROWRECORD_H
#define BORROWRECORD_H

#include "../users/User.h"
#include <string>
using namespace std;

class BorrowRecord {
private:
    User* user;
    string resourceName;
    string borrowDate;

public:
    BorrowRecord(User* user, string resourceName, string borrowDate);

    void showRecord();
};

#endif