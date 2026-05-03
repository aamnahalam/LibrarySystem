#include "NormalMembership.h"
#include <iostream>
using namespace std;

NormalMembership::NormalMembership() : Membership("Essential")
{
    borrowLimit = 2;
    borrowedCount = 0;
}

int NormalMembership::getMaxBorrowLimit() const
{
    return 2; // Basic tier: borrow up to 2 books
}

double NormalMembership::getFineDiscount() const
{
    return 0.0; // No discount on fines
}

double NormalMembership::getMonthlyPrice() const
{
    return 0.0; // Free tier
}

string NormalMembership::getPerks() const
{
    return "- Borrow up to 2 books\n"
           "  - No fine discounts\n"
           "  - Standard access to library";
}

void NormalMembership::displayDetails() const
{
    cout << "\n=== ESSENTIAL MEMBERSHIP (Free) ===" << endl;
    cout << "Max Books: " << getMaxBorrowLimit() << endl;
    cout << "Fine Discount: " << (getFineDiscount() * 100) << "%" << endl;
    cout << "Monthly Cost: Free" << endl;
    cout << "Perks:\n" << getPerks() << endl;
}