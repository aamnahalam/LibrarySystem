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
    void displayInfo() override;
    bool operator==(const User& other) const;

};

#endif