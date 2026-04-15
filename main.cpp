#include<iostream>
#include "src/core/LibrarySystem.h"
#include "src/users/User.h"
#include "src/users/Admin.h"

using namespace std;

int main() {

    LibrarySystem system;

    // Create users
    User* u1 = new User(1, "Ali", "Khan");
    User* u2 = new User(2, "Sara", "Ahmed");

    // Create admin
    Admin* a1 = new Admin(101, "Mr", "Boss");

    // Add to system
    system.addUser(u1);
    system.addUser(u2);
    system.addAdmin(a1);

    // Show data
    cout << "=== USERS ===" << endl;
    system.showAllUsers();

    cout << "\n=== ADMINS ===" << endl;
    system.showAllAdmins();

    return 0;
}