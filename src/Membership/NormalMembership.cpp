#include "NormalMembership.h"
#include <iostream>

NormalMembership::NormalMembership() : Membership("Normal")
{
}

int NormalMembership::getMaxBorrowLimit() const
{
    return 2;
}

double NormalMembership::getFineDiscount() const
{
    // No discount for Normal Membership
    return 0.0;
}
void NormalMembership::displayDetails() const
{
    cout << "Membership: Normal" << endl;
    cout << "Borrow Limit: 2" << endl;
    cout << "Fine Discount: 0%" << endl;
}
