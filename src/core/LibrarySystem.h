#ifndef LIBRARYSYSTEM_H
#define LIBRARYSYSTEM_H

#include <vector>
#include <string>
using namespace std;

class User {
public:
    string name;
    string email;
    string password;

    User(string n, string e, string p);
    void displayInfo();
};

class Admin {
public:
    string name;
    string email;
    string password;

    Admin(string n, string e, string p);
    void displayInfo();
};

class LibrarySystem {
private:
    vector<User*> users;
    vector<Admin*> admins;

public:
    LibrarySystem();

    void addUser(User* user);
    void addAdmin(Admin* admin);

    bool userLogin(string email, string password);
    bool adminLogin(string email, string password);

    void showAllUsers();
    void showAllAdmins();
};

#endif