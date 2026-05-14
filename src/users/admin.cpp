#include "admin.h"
#include "../core/LibrarySystem.h"
#include "user.h"
#include "../resources/Resource.h"
#include "../Membership/Membership.h"
#include "../Membership/NormalMembership.h"
#include "../Membership/ExtraMembership.h"
#include "../Membership/DeluxeMembership.h"
#include "../transactions/BorrowRecord.h"
#include "../exceptions/LibraryException.h"
#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

Admin::Admin(int id, string firstName, string lastName, string email, string password, string level)
    : Person(id, firstName, lastName, email, password) {
    accessLevel = level;
}

// ADD RESOURCE
void Admin::addResources(Resource *r, LibrarySystem& system) {
    if (r == nullptr) {
        throw LibraryException("Cannot add resource: Invalid resource provided.");
    }

    system.resources.push_back(r);
}
// REMOVE RESOURCE
void Admin::removeResources(int resourceID, LibrarySystem& system) {
    for (auto it = system.resources.begin(); it != system.resources.end(); ++it) {
        if ((*it)->getResourceID() == resourceID) {
            delete *it; 
            system.resources.erase(it);
            return;
        }
    }
    throw LibraryException("Cannot remove resource: Resource with ID " + to_string(resourceID) + " not found.");
}

// UPDATE RESOURCE 
void Admin::updateResources(int resourceID, LibrarySystem& system) {
    for (auto& r : system.resources) {
        if (r->getResourceID() == resourceID) {
            r->updateAvailability(true);
            return;
        }
    }
    throw LibraryException("Cannot update resource: Resource with ID " + to_string(resourceID) + " not found.");
}

// LOCK USER 
void Admin::lockUser(int userID, LibrarySystem& system) {
    for (auto u : system.users) {
        if (u && u->getID() == userID) {
            if (u->getLockStatus()) {
                u->unlock();
            } else {
                u->lock();
            }
            return;
        }
    }
    throw LibraryException("Cannot lock/unlock user: User with ID " + to_string(userID) + " not found.");
}

// CUSTOMER REPORT
void Admin::generateCustomerReport(LibrarySystem& system) {
    for (const auto user : system.users) {
        if (user) {
            cout << "ID: " << user->getID()
                 << ", Name: " << user->getFullName()
                 << ", Email: " << user->getEmail()
                 << ", Balance: " << user->getAccountBalance() << endl;
        }
    }
}

// ISSUED RESOURCES REPORT (BORROW HISTORY)
void Admin::generateIssuedResourcesReport(LibrarySystem& system) {
    bool hasRecords = false;
    cout << left << setw(15) << "User" << setw(25) << "Book Title" 
         << setw(15) << "Borrow Date" << setw(15) << "Due Date" 
         << setw(12) << "Returned" << "\n";
    cout << string(82, '-') << "\n";
    
    for (const auto user : system.users) {
        if (!user) continue;
        for (const auto& record : user->getBorrowHistory()) {
            hasRecords = true;
            cout << left << setw(15) << user->getFullName().substr(0, 14)
                 << setw(25) << record.getResourceName().substr(0, 24)
                 << setw(15) << record.getBorrowDate()
                 << setw(15) << record.getDueDate()
                 << setw(12) << (record.getReturnStatus() ? "Yes" : "No") << "\n";
        }
    }
    
    if (!hasRecords) {
        cout << "No borrow records found.\n";
    }
}

// OVERDUE REPORT
void Admin::generateOverdueResourcesReport(LibrarySystem& system) {
    bool hasRecords = false;
    for (const auto user : system.users) {
        if (!user) {
            continue;
        }
        for (const auto& record : user->getBorrowHistory()) {
            if (record.isOverdue()) {
                hasRecords = true;
                cout << "User: " << user->getFullName()
                     << ", Resource: " << record.getResourceName()
                     << ", Due Date: " << record.getDueDate() << endl;
            }
        }
    }
    if (!hasRecords) {
        cout << "No overdue resources found.\n";
    }
}

// FINE REPORT
void Admin::generateFineReport(LibrarySystem& system) {
    bool hasRecords = false;
    for (const auto user : system.users) {
        if (!user) {
            continue;
        }
        double totalFine = 0.0;

        for (const auto& record : user->getBorrowHistory()) {
            if (record.isOverdue()) {
                double resourceFineRate = 1.0;
                for (const auto& res : system.resources) {
                    if (res->getResourceID() == record.getResourceID()) {
                        resourceFineRate = res->getFineRate();
                        break;
                    }
                }
                double discountMultiplier = 1.0 - user->getFineDiscount();
                totalFine += record.calculateFine(resourceFineRate, discountMultiplier);
            }
        }

        if (totalFine > 0) {
            hasRecords = true;
            cout << "User: " << user->getFullName()
                 << ", Total Fine: Rs." << fixed << setprecision(2) << totalFine << endl;
        }
    }
    if (!hasRecords) {
        cout << "No fines to report.\n";
    }
}

void Admin::assignMembershipTier(int userID, int tier, LibrarySystem& system) {
    for (auto& u : system.users) {
        if (u && u->getID() == userID) {
            Membership* m = nullptr;
            switch (tier) {
                case 1:
                    m = new NormalMembership();
                    cout << "Admin assigned Essential membership to " << u->getFullName() << endl;
                    break;
                case 2:
                    m = new ExtraMembership();
                    cout << "Admin assigned Extra membership to " << u->getFullName() << endl;
                    break;
                case 3:
                    m = new DeluxeMembership();
                    cout << "Admin assigned Deluxe membership to " << u->getFullName() << endl;
                    break;
                default:
                    cout << "Invalid membership tier chosen." << endl;
                    return;
            }
            u->setMembership(m);
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

// GET ACCESS LEVEL
string Admin::getAccessLevel() const {
    return accessLevel;
}

// CREATE NEW USER (Admins can register new users)
void Admin::createUser(string firstName, string lastName, string email, string password, double initialBalance, LibrarySystem& system) {
    // Validate input
    if (firstName.empty() || lastName.empty() || email.empty() || password.empty()) {
        cout << "Error: All fields are required.\n";
        return;
    }
    
    if (password.length() < 6) {
        cout << "Error: Password must be at least 6 characters.\n";
        return;
    }
    
    // Check if email already exists
    for (auto& user : system.users) {
        if (user && user->getEmail() == email) {
            cout << "Error: User with this email already exists.\n";
            return;
        }
    }
    
    // Register the user through the system
    system.registerUser(firstName, lastName, email, password, initialBalance);
    cout << "User " << firstName << " " << lastName << " registered successfully.\n";
}

// CREATE NEW ADMIN (only SuperAdmins can do this)
bool Admin::createAdmin(string firstName, string lastName, string email, string password, string level, LibrarySystem& system) {
    // Only SuperAdmins can create new admins
    if (accessLevel != "SuperAdmin") {
        cout << "Error: Only SuperAdmins can create new admins. Your level: " << accessLevel << endl;
        return false;
    }

    // Use the system's registerAdmin method which also performs validation
    return system.registerAdmin(firstName, lastName, email, password, level);
}