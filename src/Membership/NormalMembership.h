#ifndef NORMALMEMBERSHIP_H
#define NORMALMEMBERSHIP_H

#include "Membership.h"

// Normal Membership: Borrow upto 2 books, no discount
class NormalMembership : public Membership
{
public:
    // Constructor for NormalMembership class:
    NormalMembership();
    // overriden functions:
    int getMaxBorrowLimit() const override;
    double getFineDiscount() const override;
    void displayDetails() const override;
};

#endif
