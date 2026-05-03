#include "DeluxeMembership.h"
#include <iostream>
using namespace std;

DeluxeMembership::DeluxeMembership() : Membership("Deluxe")
{
}

int DeluxeMembership::getMaxBorrowLimit() const
{
    return 10; // Can borrow up to 10 books
}

double DeluxeMembership::getFineDiscount() const
{
    return 0.50; // 50% discount on fines
}

double DeluxeMembership::getMonthlyPrice() const
{
    return 20.0; // $20 per month
}

string DeluxeMembership::getPerks() const
{
    return "- Borrow up to 10 books\n"
           "  - 50% discount on all fines\n"
           "  - Free fine waiver (once per month)\n"
           "  - Priority reservation with priority hold queue\n"
           "  - Early access to new arrivals (7 days ahead)\n"
           "  - Loyalty points earn at 1.5x rate\n"
           "  - Monthly newsletter with book recommendations";
}

void DeluxeMembership::displayDetails() const
{
    cout << "\n=== DELUXE MEMBERSHIP ($20/month) ===" << endl;
    cout << "Max Books: " << getMaxBorrowLimit() << endl;
    cout << "Fine Discount: " << (getFineDiscount() * 100) << "%" << endl;
    cout << "Perks:\n" << getPerks() << endl;
}
