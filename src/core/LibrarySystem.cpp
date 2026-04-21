#include "LibrarySystem.h"
#include <iostream>
using namespace std;

// ---------------- USER ----------------
User::User(string n, string e, string p) {
    name = n;
    email = e;
    password = p;
}

void User::displayInfo() {
    cout << "User Name: " << name << endl;
    cout << "Email: " << email << endl;
}

// ---------------- ADMIN ----------------
Admin::Admin(string n, string e, string p) {
    name = n;
    email = e;
    password = p;
}

void Admin::displayInfo() {
    cout << "Admin Name: " << name << endl;
    cout << "Email: " << email << endl;
}

// ---------------- LIBRARY SYSTEM ----------------
LibrarySystem::LibrarySystem() {}

void LibrarySystem::addUser(User* user) {
    users.push_back(user);
}

void LibrarySystem::addAdmin(Admin* admin) {
    admins.push_back(admin);
}

// -------- USER LOGIN --------
bool LibrarySystem::userLogin(string email, string password) {
    for (auto u : users) {
        if (u->email == email && u->password == password) {
            cout << "User Login Successful! Welcome " << u->name << endl;
            return true;
        }
    }
    cout << "User Login Failed!" << endl;
    return false;
}

// -------- ADMIN LOGIN --------
bool LibrarySystem::adminLogin(string email, string password) {
    for (auto a : admins) {
        if (a->email == email && a->password == password) {
            cout << "Admin Login Successful! Welcome " << a->name << endl;
            return true;
        }
    }
    cout << "Admin Login Failed!" << endl;
    return false;
}

void LibrarySystem::showAllUsers() {
    cout << "\n--- USERS ---\n";
    for (auto u : users) {
        u->displayInfo();
        cout << "-----------------\n";
    }
}

void LibrarySystem::showAllAdmins() {
    cout << "\n--- ADMINS ---\n";
    for (auto a : admins) {
        a->displayInfo();
        cout << "-----------------\n";
    }
}