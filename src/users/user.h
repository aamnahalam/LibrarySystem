#ifndef USER_H
#define USER_H

#include "Person.h"
#include "../transactions/BorrowRecord.h"
#include <vector>
#include <string>

using namespace std;

class Resource;
class Membership;
class BorrowRecord;

class User : public Person {
    friend class LibrarySystem;
private:
    bool isLocked;
    bool fineWaiverActive;
    int loyaltypoints;
    int readingFrequency;
    double accountbalance;
    string lastBorrowDate;
    int borrowsToday;
    string lastBorrowMonth;
    int borrowsThisMonth;
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
    double returnresources(Resource* r, string date);
    void rechargebalance(double amount);
    void earnpoints(int points);
    void setMembership(Membership* m);
    void lock();
    void unlock();
    // Profile & History Methods
    void updateprofile(string firstName, string lastName, string email, string password);
    void displayInfo() override;

    // Getters (Required for access from outside the User/Person classes)
    int getID() const;
    string getEmail() const;
    string getFullName() const;
    string getPassword() const;
    double getAccountBalance() const;
    string getPreferredCategory() const;
    int getLoyaltyPoints() const;
    string getMembershipName() const;
    double getFineDiscount() const;
    string getLastBorrowDate() const;
    int getBorrowsToday() const;
    string getLastBorrowMonth() const;
    int getBorrowsThisMonth() const;
    
    // Loyalty Points Redemption
    bool redeemPointsForDiscount(int pointsToRedeem);
    bool redeemPointsForFineFreePass();
    bool hasFineWaiverActive() const;
    void displayLoyaltySummary() const;
    
    // Membership Upgrade
    bool checkAndUpgradeMembership();
    void displayMembershipDetails() const;
    bool changeMembershipTier(int tier, bool confirm = true);
    string getMembershipChangeNotice(int tier) const;
    void showMembershipOptions() const;

    const vector<BorrowRecord>& getBorrowHistory() const;
};

#endif