#ifndef USER_H
#define USER_H

#include "Person.h"

class User : public Person {
public:
    User(int id, string firstName, string lastName);

    void borrowResource();
    void returnResource();

    void displayInfo() override;
};

#endif