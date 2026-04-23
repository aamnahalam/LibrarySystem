#ifndef USER_H
#define USER_H

#include "Person.h"
#include <string>
using namespace std;

class User : public Person
{
private:
    double accountbalance;
    int loyaltypoints;

public:
    User(int id, string firstName, string lastName,
         string email,
         string password,
         double balance);

    void rechargebalance(double balance);
    bool deductFromBalance(double amount);
    void earnpoints(int points);

    int getID() const;
    string getEmail() const;
    string getFullName() const;
    double getAccountBalance() const;

    void displayInfo() override;
};

#endif