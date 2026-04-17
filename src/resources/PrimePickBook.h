#ifndef PRIMEPICKBOOK_H
#define PRIMEPICKBOOK_H

#include "Resource.h"

class PrimePickBook : public Resource
{
public:
    PrimePickBook(int id, string title, string author, string category);

    void displayDetails();
    int getBorrowLimit();
    double getFineRate();
};

#endif
