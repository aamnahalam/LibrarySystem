#pragma once
#ifndef FREQUENTREADERMEMBERSHIP_H
#define FREQUENTREADERMEMBERSHIP_H

#include "Membership.h"

class FrequentReaderMembership : public Membership
{
public:
    FrequentReaderMembership();
    
    int getMaxBorrowLimit() const override;
    double getFineDiscount() const override;
    double getMonthlyPrice() const override;
    string getPerks() const override;
    void displayDetails() const override;
};

#endif
