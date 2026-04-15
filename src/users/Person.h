#ifndef PERSON_H
#define PERSON_H

#include <string>
using namespace std;

class Person {
protected:
    int id;
    string firstName;
    string lastName;

public:
    Person(int id, string firstName, string lastName);
    
    virtual void displayInfo() = 0; 
    // pure virtual → makes it abstract class
};

#endif