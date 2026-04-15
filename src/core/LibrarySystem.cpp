#include "LibrarySystem.h"
#include <iostream>
using namespace std;

LibrarySystem::LibrarySystem() {
}

void LibrarySystem::addUser(User* user) {
    users.push_back(user);
}

void LibrarySystem::addAdmin(Admin* admin) {
    admins.push_back(admin);
}

void LibrarySystem::showAllUsers() {
    for (auto u : users) {
        u->displayInfo();
    }
}

void LibrarySystem::showAllAdmins() {
    for (auto a : admins) {
        a->displayInfo();
    }
}