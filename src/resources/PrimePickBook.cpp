#include "PrimePickBook.h"
#include <iostream>

PrimePickBook::PrimePickBook(int id, string title, string author, string category)
    : Resource(id, title, author, category) {}

void PrimePickBook::displayDetails()
{
    cout << "Prime Pick Book\n";
    cout << "Title: " << title << endl;
    cout << "Author: " << author << endl;
}

int PrimePickBook::getBorrowLimit()
{
    return 5;
}

double PrimePickBook::getFineRate()
{
    return 5.0;
}
