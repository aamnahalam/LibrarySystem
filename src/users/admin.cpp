#include "admin.h"
#include "LibrarySystem.h"
#include "user.h"
#include "Resource.h"
#include "Membership.h"
#include "NormalMembership.h"
#include "FrequentReaderMembership.h"
#include "BorrowRecord.h"
#include <iostream>
using namespace std;

Admin::Admin(int id, string firstName, string lastName, string email, string password, string level)
    : Person(id, firstName, lastName, email, password) {
    accessLevel = level;
}

// ADD RESOURCE
void Admin::addResources(Resource *r, LibrarySystem& system) {
    if (r == nullptr) {
        cout << " Invalid Resource. " << endl;
        return;
    }

    system.resources.push_back(r);
    cout << " Resource Added Successfully. " << endl;
}

// REMOVE RESOURCE (FIXED MEMORY LEAK)
bool Admin::removeResources(int resourceID, LibrarySystem& system) {
    for (auto it = system.resources.begin(); it != system.resources.end(); ++it) {
        if ((*it)->getResourceID() == resourceID) {
            delete *it; // 🔥 IMPORTANT FIX
            system.resources.erase(it);
            cout << " Resource Removed Successfully. " << endl;
            return true;
        }
    }
    cout << " Resource Not Found. " << endl;
    return false;
}

// UPDATE RESOURCE (FIXED ACCESS)
bool Admin::updateResources(int resourceID, LibrarySystem& system) {
    for (auto& r : system.resources) {
        if (r->getResourceID() == resourceID) {
            r->updateAvailability(true);
            cout << " Resource Updated Successfully. " << endl;
            return true;
        }
    }
    cout << " Resource Not Found. " << endl;
    return false;
}

// LOCK USER (FIXED LOGIC)
void Admin::lockUser(int userID, LibrarySystem& system) {
    for (auto& u : system.users) {
        if (u.getID() == userID) {
            u.lock();  // 🔥 PROPER LOCK
            cout << " User Locked Successfully. " << endl;
            return;
        }
    }
    cout << " User Not Found. " << endl;
}

// CUSTOMER REPORT
void Admin::generateCustomerReport(LibrarySystem& system) {
    cout << "Customer Report:\n";
    for (const auto& user : system.users) {
        cout << "ID: " << user.getID()
             << ", Name: " << user.getFullName()
             << ", Email: " << user.getEmail()
             << ", Balance: " << user.getAccountBalance() << endl;
    }
}

// ISSUED RESOURCES REPORT (FIXED ACCESS)
void Admin::generateIssuedResourcesReport(LibrarySystem& system) {
    cout << "Issued Resources Report:\n";
    for (const auto& resource : system.resources) {
        if (!resource->getAvailability()) {
            cout << "Resource ID: " << resource->getResourceID()
                 << ", Title: " << resource->getTitle() << endl;
        }
    }
}

// OVERDUE REPORT
void Admin::generateOverdueResourcesReport(LibrarySystem& system) {
    cout << "Overdue Resources Report:\n";
    for (const auto& user : system.users) {
        for (const auto& record : user.getBorrowHistory()) {
            if (record.isOverdue()) {
                cout << "User: " << user.getFullName()
                     << ", Resource: " << record.getResourceName()
                     << ", Due Date: " << record.getDueDate() << endl;
            }
        }
    }
}

// FINE REPORT
void Admin::generateFineReport(LibrarySystem& system) {
    cout << "Fine Report:\n";
    for (const auto& user : system.users) {
        double totalFine = 0.0;

        for (const auto& record : user.getBorrowHistory()) {
            if (record.isOverdue()) {
                totalFine += record.calculateFine();
            }
        }

        if (totalFine > 0) {
            cout << "User: " << user.getFullName()
                 << ", Total Fine: $" << totalFine << endl;
        }
    }
}

// APPROVE DIGITAL UPLOAD (FIXED ACCESS)
void Admin::approveDigitalUpload(int resourceID, LibrarySystem& system) {
    for (auto& r : system.resources) {
        if (r->getResourceID() == resourceID) {
            r->digitalAvailable = true; // allowed via friend
            cout << " Digital Upload Approved Successfully. " << endl;
            return;
        }
    }
    cout << " Resource Not Found. " << endl;
}

// ASSIGN MEMBERSHIP (LOGIC FIXED)
void Admin::assignCardType(int userID, LibrarySystem& system) {
    for (auto& u : system.users) {
        if (u.getID() == userID) {

            if (u.getAccountBalance() > 100) {
                u.setMembership(new FrequentReaderMembership());
                cout << " Gold Card Assigned Successfully. " << endl;
            } 
            else if (u.getAccountBalance() > 50) {
                u.setMembership(new FrequentReaderMembership());
                cout << " Silver Card Assigned Successfully. " << endl;
            } 
            else {
                u.setMembership(new NormalMembership());
                cout << " Bronze Card Assigned Successfully. " << endl;
            }

            return;
        }
    }
    cout << " User Not Found. " << endl;
}

// DISPLAY ADMIN INFO
void Admin::displayInfo() {
    cout << "ID: " << getID() << endl;
    cout << "Name: " << getFullName() << endl;
    cout << "Email: " << getEmail() << endl;
    cout << "Access Level: " << accessLevel << endl;
}