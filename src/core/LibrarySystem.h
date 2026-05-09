#ifndef LIBRARYSYSTEM_H
#define LIBRARYSYSTEM_H

#include <vector>
#include <string>
using namespace std;

class User;
class Admin;
class Resource;
class BorrowRecord;

class LibrarySystem
{
public:
    vector<User *> users;
    vector<Admin *> admins;
    vector<Resource *> resources;
    vector<BorrowRecord *> borrowRecords;

private:
    User *currentUser;
    Admin *currentAdmin;

public:
    LibrarySystem();
    ~LibrarySystem();

    void addUser(User *user);
    void addAdmin(Admin *admin);

    // Register new user
    void registerUser(string firstName, string lastName, string email, string password, double initialBalance = 0.0);
    
    // Register new admin (only for SuperAdmins)
    bool registerAdmin(string firstName, string lastName, string email, string password, string level);

    // User management
    bool authenticate(string email, string password);
    void logout();
    User *getCurrentUser() const;
    Admin *getCurrentAdmin() const;
    bool changeUserMembershipTier(int userID, int tier, bool confirm = true);

    // Resource operations
    Resource *getResourceByID(int resourceID) const;
    bool borrowResource(int resourceID, string date);
    double returnResource(int resourceID, string date);

    // Reporting
    void showAllUsers() const;
    void showAllAdmins() const;

    // Data persistence
    void saveData();
    void loadData();
    friend class Admin;
};

#endif