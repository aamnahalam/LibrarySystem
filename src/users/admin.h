#ifndef ADMIN_H
#define ADMIN_H

#include "Person.h"
#include<string>
using namespace std;

class Resource;
class User;
class LibrarySystem;

class Admin : public Person {
private:
string accessLevel;
public:
    Admin(int id, string firstName, string lastName, string email,
    string password,
    string level);

    void addResources(Resource *r, LibrarySystem& system);
    bool removeResources(int resourceID, LibrarySystem& system);
    bool updateResources(int resourceID, LibrarySystem& system);

    void lockUser(int userID, LibrarySystem& system);

    
  void generateCustomerReport(LibrarySystem& system);
    void generateIssuedResourcesReport(LibrarySystem& system);
    void generateOverdueResourcesReport(LibrarySystem& system);
    void generateFineReport(LibrarySystem& system);

    
    void approveDigitalUpload(int resourceID, LibrarySystem& system);
    void assignCardType(int userID, LibrarySystem& system);


    void displayInfo() override;

};

#endif