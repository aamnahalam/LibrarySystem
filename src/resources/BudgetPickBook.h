#ifndef BUDGETPICKBOOK_H
#define BUDGETPICKBOOK_H

#include "Resource.h"

class BudgetPickBook : public Resource
{
public:
    BudgetPickBook(int id, string title, string author, string category);

    void displayDetails();
    int getBorrowLimit();
    double getFineRate();
};

#endif
