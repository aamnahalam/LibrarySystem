#ifndef LIBRARYSYSTEM_H
#define LIBRARYSYSTEM_H

#include <vector>
#include <string>
#include "Resource.h"

class User {
private:
    std::string name;
    std::string email;
    std::string password;

public:
    User(std::string n, std::string e, std::string p);
    void displayInfo();

    std::string getEmail();
    std::string getPassword();
};

class Admin {
private:
    std::string name;
    std::string email;
    std::string password;

public:
    Admin(std::string n, std::string e, std::string p);
    void displayInfo();

    std::string getEmail();
    std::string getPassword();
};

class LibrarySystem {
private:
    std::vector<User*> users;
    std::vector<Admin*> admins;
    std::vector<Resource*> resources;

    User* currentUser;
    Admin* currentAdmin;

public:
    LibrarySystem();
    ~LibrarySystem();

    void addUser(User* user);
    void addAdmin(Admin* admin);
    void addResource(Resource* resource);

    bool userLogin(std::string email, std::string password);
    bool adminLogin(std::string email, std::string password);

    void showAllUsers() const;
    void showAllAdmins() const;
    void showAllResources() const;

    
    void borrowResource(int resourceID);
    void returnResource(int resourceID);
};

#endif