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

public:
    LibrarySystem();
    ~LibrarySystem();

    void addUser(User *user);
    void addAdmin(Admin *admin);

    // User management
    bool authenticate(string email, string password);
    void logout();

    // Resource search & filters
    vector<Resource *> searchResource(string keyword);
    vector<Resource *> filterResources(string category);
    vector<Resource *> filterByAvailability();
    vector<Resource *> filterByNewArrivals();
    vector<Resource *> filterByMostBorrowed();
    vector<Resource *> filterByRating();
    vector<Resource *> filterByUserPreference(User *u);

    // Reporting
    void showAllUsers() const;
    void showAllAdmins() const;

    // Data persistence
    void saveData();
    void loadData();
    friend class Admin;
};

#endif