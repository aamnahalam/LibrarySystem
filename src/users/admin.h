#ifndef ADMIN_H
#define ADMIN_H

#include "Person.h"
#include<string>
using namespace std;

class Admin : public Person {
private:
string accessLevel;
public:
    Admin(int id, string firstName, string lastName, string email,
    string password,
    string level);

    // void manageUsers();
    // void manageResources();

    void displayInfo() override;
};

#endif