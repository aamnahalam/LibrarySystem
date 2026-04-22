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
// Add points
void User::earnpoints(int points)
{
    loyaltypoints += points;
}

void User::displayInfo()
{
    cout << "User Name: " << firstName << " " << lastName << endl;
    cout << "Email: " << email << endl;
    cout << "Balance: " << accountbalance << endl;
    cout << "Loyalty Points: " << loyaltypoints << endl;
}