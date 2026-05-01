#include "LibrarySystem.h"
#include "User.h"
#include "Resource.h"
#include "BorrowRecord.h"

#include <algorithm>

LibrarySystem::LibrarySystem() {
    currentUser = nullptr;
}

// Register user
bool LibrarySystem::registerUser(User u) {
    users.push_back(u);
    return true;
}

// Authenticate user
bool LibrarySystem::authenticate(std::string email, std::string password) {
    for (auto &user : users) {
        if (user.getEmail() == email && user.getPassword() == password) {
            currentUser = &user;
            return true;
        }
    }
    return false;
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
        if (r->isAvailable()) {
            result.push_back(r);
        }
    }

    return result;
}

// Filter new arrivals
std::vector<Resource*> LibrarySystem::filterByNewArrivals() {
    std::vector<Resource*> result;

    for (auto r : resources) {
        if (r->isNewArrival()) {
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
        return a->getRating() > b->getRating();
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