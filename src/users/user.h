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
    bool isLocked;
    int loyaltypoints;
    int readingFrequency;
    double accountbalance;
    Membership* membership;
    vector<Resource*> borrowedResources;
    vector<BorrowRecord> borrowHistory;
    vector<string> favouriteCategories;

public:
    // Construtor
    User(int id, string firstName, string lastName, string email, string password, double balance);
    
    ~User();
    // Core Logic Methods
    bool borrowresources(Resource* r, string date);
    bool getLockStatus() const;
    bool deductFromBalance(double amount);
    double returnresources(Resource* r, string date);
    void rechargebalance(double amount);
    void earnpoints(int points);
    void setMembership(Membership* m);
    void lock();
    // Profile & History Methods
    void viewhistory();
    void updateprofile(string firstName, string lastName, string email, string password);
    void displayInfo() override;

    // Getters (Required for access from outside the User/Person classes)
    int getID() const;
    string getEmail() const;
    string getFullName() const;
    string getPassword() const;
    double getAccountBalance() const;
    string getPreferredCategory() const;
    // Operator Overloading
    bool operator==(const User& other) const;

    const vector<BorrowRecord>& getBorrowHistory() const;
};

#endif