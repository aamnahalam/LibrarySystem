#include<iostream>
#include "src/core/LibrarySystem.h"
#include "src/users/user.h"
#include "src/users/admin.h"

using namespace std;

int main() {

    LibrarySystem system;

    // Create users
    User* u1 = new User(1,"Ali", "Khan", "ali@email.com", "1234", 78.0);
    User* u2 = new User(2, "Sara", "Ahmed", "sara@gmail.com", "5678", 150.0);

    // Create admin
    Admin* a1 = new Admin(101, "Mr", "Boss", "admin@library.com", "admin123", "SuperAdmin");

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