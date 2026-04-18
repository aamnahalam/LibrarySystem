#ifndef USER_H
#define USER_H

#include "Person.h"
#include<string>
using namespace std;

class User : public Person {
private:
double accountbalance;
int loyaltypoints;
public:
    User(int id, string firstName, string lastName, 
    string email,
    string password,
    double balance);

    void rechargebalance(double balance);
    void earnpoints(int points);

    void displayInfo() override;
};

#endif