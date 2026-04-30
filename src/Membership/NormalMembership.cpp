#include "NormalMembership.h"
#include <iostream>
using namespace std;

NormalMembership::NormalMembership() : Membership("Normal")   // 🔥 THIS IS REQUIRED
{
    borrowLimit = 3;
    borrowedCount = 0;
}
int NormalMembership::getMaxBorrowLimit() const {
    return 3;
}

double NormalMembership::getFineDiscount() const {
    return 0.0;
}

void NormalMembership::displayDetails() const {
    cout << "Normal Membership\n";
}