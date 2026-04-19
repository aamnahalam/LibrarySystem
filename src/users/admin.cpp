#include "admin.h"
#include <iostream>
using namespace std;

Admin::Admin(int id, string firstName, string lastName, string email,
 string password, string level)
    : Person(id, firstName, lastName,email,password) {
        accessLevel=level;
}

// void Admin::manageUsers() {
// }

// void Admin::manageResources() {
// }

void Admin::displayInfo() {
    cout << "ID: " <<id<< endl;
    cout << "Name: " << firstName << " " << lastName << endl;
    cout << "Email: " <<email<< endl;
    cout << "Acces Level: " <<accessLevel<< endl;
}