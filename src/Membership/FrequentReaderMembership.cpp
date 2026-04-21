#include "FrequentReaderMembership.h"
#include <iostream>

FrequentReaderMembership::FrequentReaderMembership() : Membership("Frequent Reader")
{
}

int FrequentReaderMembership::getMaxBorrowLimit() const
{
    return 2;
}
double FrequentReaderMembership::getFineDiscount() const
{
    // 20% discount for Frequent Reader Membership
    return 0.20;
}
void FrequentReaderMembership::displayDetails() const
{
    cout << "Membership: Frequent Reader" << endl;
    cout << "Borrow Limit: 2" << endl;
    cout << "Fine Discount: 20%" << endl;
}
