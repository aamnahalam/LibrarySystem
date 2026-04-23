#ifndef USER_H
#define USER_H

#include "Person.h"
#include<vector>
#include<string>
using namespace std;

class Resource;
class Membership;
class BorrowRecord;

class User : public Person {
private:
double accountbalance;
int loyaltypoints;
Membership* membership;
vector<Resource*>borrowedResources;
vector<BorrowRecord>borrowHistory;
vector<string>favouriteCategories;

int readingFrequency;

public:
    User(int id, string firstName, string lastName, 
    string email,
    string password,
    double balance);
   
    bool borrowresources(Resource* r, string date);
    void rechargebalance(double balance);
    double returnresources(Resource* r, string date);
    void earnpoints(int points);
    void setMembership(Membership*m);
    void updateprofile(string firstName, string lastName, string email, string password);
    void viewhistory();
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
    bool operator==(const User& other) const;

};

#endif