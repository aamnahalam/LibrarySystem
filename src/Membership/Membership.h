#ifndef MEMBERSHIP_H
#define MEMBERSHIP_H

#include <string>
using namespace std;
// Abstract Base class for Membership types:
class Membership
{
public:
    // Constructor for Membership class:
    Membership(const string &levelName);

    // Destructor:
    virtual ~Membership() = default;

    // Method to get level name:
    string getLevelName() const;

    // Pure virtual function to get Max Borrow Limit:
    virtual int getMaxBorrowLimit() const = 0;

    // Pure virtual function to get Fine Discount:
    virtual double getFineDiscount() const = 0;

    // Pure virtual function to Display Details:
    virtual void displayDetails() const = 0;

protected:
    // Attributes
    string levelName;
};

#endif
