#include "Admin.h"
#include <iostream>
using namespace std;

Admin::Admin(int id, string firstName, string lastName)
    : Person(id, firstName, lastName) {
}

void Admin::manageUsers() {
}

void Admin::manageResources() {
}

void Admin::displayInfo() {
    cout << "Admin: " << firstName << " " << lastName << endl;
}