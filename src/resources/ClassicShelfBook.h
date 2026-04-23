#ifndef CLASSICSHELFBOOK_H
#define CLASSICSHELFBOOK_H

#include "Resource.h"

class ClassicShelfBook : public Resource
{
public:
    ClassicShelfBook(int id, string title, string author, string category);

    void displayDetails();
    int getBorrowLimit();
    double getFineRate();
};

#endif