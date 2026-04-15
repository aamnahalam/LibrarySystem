#include "User.h"
#include <iostream>
using namespace std;

User::User(int id, string firstName, string lastName)
    : Person(id, firstName, lastName) {}

void User::borrowResource() {
}

void User::returnResource() {
}

void User::displayInfo() {
    cout << "User: " << firstName << " " << lastName << endl;
}