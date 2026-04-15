#ifndef ADMIN_H
#define ADMIN_H

#include "Person.h"

class Admin : public Person {
public:
    Admin(int id, string firstName, string lastName);

    void manageUsers();
    void manageResources();

    void displayInfo() override;
};

#endif