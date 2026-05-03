#include "ExtraMembership.h"
#include <iostream>
using namespace std;

ExtraMembership::ExtraMembership() : Membership("Extra")
{
}

int ExtraMembership::getMaxBorrowLimit() const
{
    return 5; // Can borrow up to 5 books
}

double ExtraMembership::getFineDiscount() const
{
    return 0.25; // 25% discount on fines
}

double ExtraMembership::getMonthlyPrice() const
{
    return 10.0; // $10 per month
}

string ExtraMembership::getPerks() const
{
    return "- Borrow up to 5 books\n"
           "  - 25% discount on all fines\n"
           "  - Priority reservation\n"
           "  - Early access to new arrivals";
}

void ExtraMembership::displayDetails() const
{
    cout << "\n=== EXTRA MEMBERSHIP ($10/month) ===" << endl;
    cout << "Max Books: " << getMaxBorrowLimit() << endl;
    cout << "Fine Discount: " << (getFineDiscount() * 100) << "%" << endl;
    cout << "Perks:\n" << getPerks() << endl;
}
