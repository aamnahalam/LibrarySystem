#ifndef LIBRARYSYSTEM_H
#define LIBRARYSYSTEM_H

#include <vector>
#include <string>
using namespace std;

class User;
class Admin;
class Resource;
class BorrowRecord;

class LibrarySystem {
public:
    std::vector<User*> users;
    std::vector<Admin*> admins;
    std::vector<Resource*> resources;
    std::vector<BorrowRecord*> borrowRecords;
    User* currentUser;

    LibrarySystem();
    ~LibrarySystem();

    void addUser(User* user);
    void addAdmin(Admin* admin);

    // User management
    bool authenticate(std::string email, std::string password);
    void logout();

    // Resource search & filters
    std::vector<Resource*> searchResource(std::string keyword);
    std::vector<Resource*> filterResources(std::string category);
    std::vector<Resource*> filterByAvailability();
    std::vector<Resource*> filterByNewArrivals();
    std::vector<Resource*> filterByMostBorrowed();
    std::vector<Resource*> filterByRating();
    std::vector<Resource*> filterByUserPreference(User* u);

    // Reporting
    void showAllUsers() const;
    void showAllAdmins() const;

    // Data persistence
    void saveData();
    void loadData();
};

#endif