#include<iostream>
#include "Person.h"

Person::Person(int id, string firstName, string lastName, string email, string password) {
    this->id = id;
    this->firstName = firstName;
    this->lastName = lastName;
    this->email=email;
    this->password=password;
}

//Login Function
bool Person::login(string email, string password){
    if(this->email==email && this->password==password){
        cout<< "Login Successfull. Welcome!" <<endl;
        return true;
    } 
    else{
        cout<< "Login Failed. Invalid Email Or Password." <<endl;
        return false;
    }
}

//Logout Function
void Person::logout(){
    cout<< "Logged Out Successfully." <<endl;
}

// Destructor
Person::~Person(){}

// Getters for derived classes
int Person::getID() const {
    return id;
}

string Person::getEmail() const {
    return email;
}

string Person::getFullName() const {
    return firstName + " " + lastName;
}

string Person::getPassword() const {
    return password;
}