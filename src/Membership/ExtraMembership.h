#pragma once
#ifndef EXTRAMEMBERSHIP_H
#define EXTRAMEMBERSHIP_H

#include "Membership.h"

class ExtraMembership : public Membership
{
public:
    ExtraMembership();
    
    int getMaxBorrowLimit() const override;
    double getFineDiscount() const override;
    double getMonthlyPrice() const override;
    string getPerks() const override;
    void displayDetails() const override;
};

#endif
