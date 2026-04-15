#ifndef LIBRARYSYSTEM_H
#define LIBRARYSYSTEM_H

#include <vector>
#include "../users/User.h"
#include "../users/Admin.h"
class LibrarySystem {
private:
    vector<User*> users;
    vector<Admin*> admins;

public:
    LibrarySystem();

    void addUser(User* user);
    void addAdmin(Admin* admin);

    void showAllUsers();
    void showAllAdmins();
};

#endif