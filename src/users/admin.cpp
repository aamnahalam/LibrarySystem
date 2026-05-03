#include "admin.h"
#include "../core/LibrarySystem.h"
#include "user.h"
#include "../resources/Resource.h"
#include "../Membership/Membership.h"
#include "../Membership/NormalMembership.h"
#include "../Membership/FrequentReaderMembership.h"
#include "../transactions/BorrowRecord.h"
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
// REMOVE RESOURCE
bool Admin::removeResources(int resourceID, LibrarySystem& system) {
    for (auto it = system.resources.begin(); it != system.resources.end(); ++it) {
        if ((*it)->getResourceID() == resourceID) {
            delete *it; 
            system.resources.erase(it);
            cout << " Resource Removed Successfully. " << endl;
            return true;
        }
    }
    cout << " Resource Not Found. " << endl;
    return false;
}

// UPDATE RESOURCE 
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

// LOCK USER 
void Admin::lockUser(int userID, LibrarySystem& system) {
    for (auto u : system.users) {
        if (u && u->getID() == userID) {
            u->lock(); 
            cout << " User Locked Successfully. " << endl;
            return;
        }
    }
    cout << " User Not Found. " << endl;
}

// CUSTOMER REPORT
void Admin::generateCustomerReport(LibrarySystem& system) {
    cout << "Customer Report:\n";
    for (const auto user : system.users) {
        if (user) {
            cout << "ID: " << user->getID()
                 << ", Name: " << user->getFullName()
                 << ", Email: " << user->getEmail()
                 << ", Balance: " << user->getAccountBalance() << endl;
        }
    }
}

// ISSUED RESOURCES REPORT
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
    for (const auto user : system.users) {
        if (!user) {
            continue;
        }
        for (const auto& record : user->getBorrowHistory()) {
            if (record.isOverdue()) {
                cout << "User: " << user->getFullName()
                     << ", Resource: " << record.getResourceName()
                     << ", Due Date: " << record.getDueDate() << endl;
            }
        }
    }
}

// FINE REPORT
void Admin::generateFineReport(LibrarySystem& system) {
    cout << "Fine Report:\n";
    for (const auto user : system.users) {
        if (!user) {
            continue;
        }
        double totalFine = 0.0;

        for (const auto& record : user->getBorrowHistory()) {
            if (record.isOverdue()) {
                totalFine += record.calculateFine();
            }
        }

        if (totalFine > 0) {
            cout << "User: " << user->getFullName()
                 << ", Total Fine: $" << totalFine << endl;
        }
    }
}

// APPROVE DIGITAL UPLOAD
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

// ASSIGN MEMBERSHIP 
void Admin::assignCardType(int userID, LibrarySystem& system) {
    for (auto& u : system.users) {
        if (u && u->getID() == userID) {
            int totalBorrows = (int)u->getBorrowHistory().size();
            if (totalBorrows >= 10) {
                u->setMembership(new FrequentReaderMembership());
                cout << "Frequent Reader Membership assigned to " << u->getFullName()
                     << " (" << totalBorrows << " borrows)" << endl;
            } else {
                u->setMembership(new NormalMembership());
                cout << "Normal Membership assigned to " << u->getFullName()
                     << " (" << totalBorrows << " borrows)" << endl;
            }
            return;
        }
    }
    cout << "User Not Found." << endl;
}

// DISPLAY ADMIN INFO
void Admin::displayInfo() {
    cout << "ID: " << getID() << endl;
    cout << "Name: " << getFullName() << endl;
    cout << "Email: " << getEmail() << endl;
    cout << "Access Level: " << accessLevel << endl;
}