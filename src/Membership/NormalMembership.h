#ifndef NORMALMEMBERSHIP_H
#define NORMALMEMBERSHIP_H

#include "Membership.h"

class NormalMembership : public Membership {

private:
    int borrowLimit;        // ✅ MUST exist
    int borrowedCount;      // ✅ MUST exist

public:
    NormalMembership();

    int getMaxBorrowLimit() const override;
    double getFineDiscount() const override;
    void displayDetails() const override;
};

#endif