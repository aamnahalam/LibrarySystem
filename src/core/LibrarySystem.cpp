#include "LibrarySystem.h"
#include <iostream>

// ---------------- LIBRARY SYSTEM ----------------
LibrarySystem::LibrarySystem() {
    currentUser = nullptr;
    currentAdmin = nullptr;
}

// Destructor to prevent memory leaks
LibrarySystem::~LibrarySystem() {
    for (auto u : users) delete u;
    for (auto a : admins) delete a;
    for (auto r : resources) delete r;
}

// -------- ADD FUNCTIONS --------
void LibrarySystem::addUser(User* user) {
    users.push_back(user);
}

void LibrarySystem::addAdmin(Admin* admin) {
    admins.push_back(admin);
}

void LibrarySystem::addResource(Resource* resource) {
    resources.push_back(resource);
}

// -------- USER LOGIN --------
bool LibrarySystem::userLogin(std::string email, std::string password) {
    for (auto u : users) {
        if (u->getEmail() == email && u->getPassword() == password) {
            currentUser = u;
            std::cout << "User Login Successful! Welcome " << std::endl;
            return true;
        }
    }
    std::cout << "User Login Failed!" << std::endl;
    return false;
}

// -------- ADMIN LOGIN --------
bool LibrarySystem::adminLogin(std::string email, std::string password) {
    for (auto a : admins) {
        if (a->getEmail() == email && a->getPassword() == password) {
            currentAdmin = a;
            std::cout << "Admin Login Successful! Welcome " << std::endl;
            return true;
        }
    }
    std::cout << "Admin Login Failed!" << std::endl;
    return false;
}

// -------- DISPLAY FUNCTIONS --------
void LibrarySystem::showAllUsers() const {
    std::cout << "\n--- USERS ---\n";
    for (auto u : users) {
        u->displayInfo();
        std::cout << "-----------------\n";
    }
}

void LibrarySystem::showAllAdmins() const {
    std::cout << "\n--- ADMINS ---\n";
    for (auto a : admins) {
        a->displayInfo();
        std::cout << "-----------------\n";
    }
}

void LibrarySystem::showAllResources() const {
    std::cout << "\n--- RESOURCES ---\n";
    for (auto r : resources) {
        r->displayDetails();   // assuming Resource has this
        std::cout << "-----------------\n";
    }
}

// -------- BASIC BORROW FUNCTION --------
void LibrarySystem::borrowResource(int resourceID) {
    if (!currentUser) {
        std::cout << "Please login as user first!\n";
        return;
    }

    for (auto r : resources) {
        if (r->getResourceID() == resourceID) {
            if (r->getAvailability()) {
                r->updateAvailability(false);
                std::cout << "Resource borrowed successfully!\n";
                return;
            } else {
                std::cout << "Resource not available!\n";
                return;
            }
        }
    }

    std::cout << "Resource not found!\n";
}

// -------- BASIC RETURN FUNCTION --------
void LibrarySystem::returnResource(int resourceID) {
    for (auto r : resources) {
        if (r->getResourceID() == resourceID) {
            r->updateAvailability(true);
            std::cout << "Resource returned successfully!\n";
            return;
        }
    }

    std::cout << "Resource not found!\n";
}