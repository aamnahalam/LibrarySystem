#ifndef LIBRARYSYSTEM_H
#define LIBRARYSYSTEM_H

#include <vector>
#include <string>

class User;
class Resource;
class BorrowRecord;

class LibrarySystem {
private:
    std::vector<User> users;
    std::vector<Resource*> resources;
    std::vector<BorrowRecord> borrowRecords;
    User* currentUser;

public:
    LibrarySystem();

    // User management
    bool registerUser(User u);
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

    // Data persistence
    void saveData();
    void loadData();
};

#endif