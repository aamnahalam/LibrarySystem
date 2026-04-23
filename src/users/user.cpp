#include "user.h"
#include <iostream>
using namespace std;

User::User(int id, string firstName, string lastName, string email, string password, double balance)
    : Person(id, firstName, lastName, email, password) {}

//  Add Balance
void User::rechargebalance(double amount)
{
    accountbalance += amount;
}
// Deduct from balance
bool User::deductFromBalance(double amount)
{
    if (accountbalance >= amount)
    {
        accountbalance -= amount;
        return true;
    }
    return false;
}
// Add points
void User::earnpoints(int points)
{
    loyaltypoints += points;
}

int User::getID()const{
    return id;
}
string User::getEmail() const
{
    return email;
}
string User::getFullName() const
{
    return firstName + " " + lastName;
}
double User::getAccountBalance() const
{
    return accountbalance;
}

void User::displayInfo()
{
    cout << "User Name: " << firstName << " " << lastName << endl;
    cout << "Email: " << email << endl;
    cout << "Balance: " << accountbalance << endl;
    cout << "Loyalty Points: " << loyaltypoints << endl;
}