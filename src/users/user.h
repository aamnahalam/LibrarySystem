#ifndef USER_H
#define USER_H

#include "Person.h"
#include <vector>
#include <string>

using namespace std;


class Resource;
class Membership;
class BorrowRecord;

class User : public Person {
private:
    double accountbalance;
    int loyaltypoints;
    Membership* membership;
    vector<Resource*> borrowedResources;
    vector<BorrowRecord> borrowHistory;
    int readingFrequency;
    
   
    vector<string> favouriteCategories;

public:
    // Construtor
    User(int id, string firstName, string lastName, 
         string email, string password, double balance);

    // Core Logic Methods
    bool borrowresources(Resource* r, string date);
    double returnresources(Resource* r, string date);
    void rechargebalance(double amount);
    bool deductFromBalance(double amount);
    void earnpoints(int points);
    void setMembership(Membership* m);
    
    // Profile & History Methods
    void viewhistory();
    void updateprofile(string firstName, string lastName, string email, string password);
    void displayInfo();

    // Getters (Required for access from outside the User/Person classes)
    int getID() const;
    string getEmail() const;
    string getFullName() const;
    double getAccountBalance() const;

    // Operator Overloading
    bool operator==(const User& other) const;

    

    const vector<BorrowRecord>& getBorrowHistory() const;
};

#endif