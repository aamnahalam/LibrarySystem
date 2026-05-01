#ifndef LIBRARYSYSTEM_H
#define LIBRARYSYSTEM_H

#include <vector>
#include <string>
#include "Resource.h"
#include "../users/user.h"
#include "../users/admin.h"

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

    // Friend classes for admin access
    friend class Admin;
};

#endif