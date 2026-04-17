#include "ClassicShelfBook.h"
#include <iostream>

ClassicShelfBook::ClassicShelfBook(int id, string title, string author, string category)
    : Resource(id, title, author, category) {}

void ClassicShelfBook::displayDetails()
{
    cout << "Classic Shelf Book\n";
    cout << "Title: " << title << endl;
    cout << "Author: " << author << endl;
}

int ClassicShelfBook::getBorrowLimit()
{
    return 2;
}

double ClassicShelfBook::getFineRate()
{
    return 3.0;
}
