#ifndef PERSON_H
#define PERSON_H

#include <string>
#include <iostream>
using namespace std;

class Person {
protected:
    int id;
    string firstName;
    string lastName;
    string email;
    string password;

public:
    Person(int id, string firstName, string lastName, string email, string password);
    
    bool login(string email, string password);
    void logout();
    virtual void displayInfo() = 0; 
    // pure virtual → makes it abstract class
    virtual  ~Person();

    // Getters for derived classes
    int getID() const;
    string getEmail() const;
    string getFullName() const;
    string getPassword() const;
};

#endif