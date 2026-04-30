#include "BudgetPickBook.h"
#include <iostream>

BudgetPickBook::BudgetPickBook(int id, string title, string author, string category)
    : Resource(id, title, author, category) {}

void BudgetPickBook::displayDetails()
{
    cout << "Budget Pick Book\n";
    cout << "Title: " << title << endl;
    cout << "Author: " << author << endl;
}

int BudgetPickBook::getBorrowLimit()
{
    return 3;
}

double BudgetPickBook::getFineRate()
{
    return 2.0;
}