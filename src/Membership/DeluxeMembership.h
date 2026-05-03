#pragma once
#ifndef DELUXEMEMBERSHIP_H
#define DELUXEMEMBERSHIP_H

#include "Membership.h"

class DeluxeMembership : public Membership
{
public:
    DeluxeMembership();
    
    int getMaxBorrowLimit() const override;
    double getFineDiscount() const override;
    double getMonthlyPrice() const override;
    string getPerks() const override;
    void displayDetails() const override;
};

#endif
