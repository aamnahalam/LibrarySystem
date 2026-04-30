#pragma once
#ifndef FREQUENTREADERMEMBERSHIP_H
#define FREQUENTREADERMEMBERSHIP_H

#include "Membership.h"
// Frequent Reader Membership: Borrow upto 2 books, 20% discount
class FrequentReaderMembership : public Membership
{
public:
    // Constructor for FrequentReaderMembership class:
    FrequentReaderMembership();
    // overriden functions:
    int getMaxBorrowLimit() const override;
    double getFineDiscount() const override;
    void displayDetails() const override;
};

#endif
