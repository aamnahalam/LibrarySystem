#include "LibrarySystem.h"
#include "User.h"
#include "Resource.h"
#include "BorrowRecord.h"
#include "../users/admin.h"

#include <algorithm>

LibrarySystem::LibrarySystem() {
    currentUser = nullptr;
}

LibrarySystem::~LibrarySystem() {
    for (auto user : users) {
        delete user;
    }
    for (auto admin : admins) {
        delete admin;
    }
    for (auto resource : resources) {
        delete resource;
    }
    for (auto record : borrowRecords) {
        delete record;
    }
}

void LibrarySystem::addUser(User* user) {
    if (user) {
        users.push_back(user);
    }
}

void LibrarySystem::addAdmin(Admin* admin) {
    if (admin) {
        admins.push_back(admin);
    }
}

// Authenticate user
bool LibrarySystem::authenticate(std::string email, std::string password) {
    for (auto user : users) {
        if (user->getEmail() == email && user->getPassword() == password) {
            currentUser = user;
            return true;
        }
    }
    return false;
}

void LibrarySystem::showAllUsers() const {
    for (const auto user : users) {
        if (user) {
            user->displayInfo();
        }
    }
}

void LibrarySystem::showAllAdmins() const {
    for (const auto admin : admins) {
        if (admin) {
           admin->displayInfo();
        }
    }
}

// Logout
void LibrarySystem::logout() {
    currentUser = nullptr;
}

// Search resource by keyword
std::vector<Resource*> LibrarySystem::searchResource(std::string keyword) {
    std::vector<Resource*> result;

    for (auto r : resources) {
        if (r->getTitle().find(keyword) != std::string::npos) {
            result.push_back(r);
        }
    }

    return result;
}

// Filter by category
std::vector<Resource*> LibrarySystem::filterResources(std::string category) {
    std::vector<Resource*> result;

    for (auto r : resources) {
        if (r->getCategory() == category) {
            result.push_back(r);
        }
    }

    return result;
}

// Filter available resources
std::vector<Resource*> LibrarySystem::filterByAvailability() {
    std::vector<Resource*> result;

    for (auto r : resources) {
        if (r->getAvailability()) {
            result.push_back(r);
        }
    }

    return result;
}

// Filter new arrivals
std::vector<Resource*> LibrarySystem::filterByNewArrivals() {
    std::vector<Resource*> result;

    for (auto r : resources) {
        if (r->getIsNewArrival()) {
            result.push_back(r);
        }
    }

    return result;
}

// Filter most borrowed
std::vector<Resource*> LibrarySystem::filterByMostBorrowed() {
    std::vector<Resource*> result = resources;

    std::sort(result.begin(), result.end(), [](Resource* a, Resource* b) {
        return a->getBorrowCount() > b->getBorrowCount();
    });

    return result;
}

// Filter by rating
std::vector<Resource*> LibrarySystem::filterByRating() {
    std::vector<Resource*> result = resources;

    std::sort(result.begin(), result.end(), [](Resource* a, Resource* b) {
        return a->getReviewScore() > b->getReviewScore();
    });

    return result;
}

// Filter by user preference
std::vector<Resource*> LibrarySystem::filterByUserPreference(User* u) {
    std::vector<Resource*> result;

    for (auto r : resources) {
        if (r->getCategory() == u->getPreferredCategory()) {
            result.push_back(r);
        }
    }

    return result;
}

// Save data (placeholder)
void LibrarySystem::saveData() {
    // Implement file/database saving logic
}

// Load data (placeholder)
void LibrarySystem::loadData() {
    // Implement file/database loading logic
}