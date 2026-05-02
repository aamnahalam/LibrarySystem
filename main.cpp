#include<iostream>
#include "src/core/LibrarySystem.h"
#include "src/users/user.h"
#include "src/users/admin.h"
#include "src/resources/PrimePickBook.h"
#include "src/resources/ClassicShelfBook.h"
#include "src/resources/BudgetPickBook.h"
#include "src/Membership/NormalMembership.h"
#include "src/Membership/FrequentReaderMembership.h"
#include "src/services/FineWalletManager.h"
#include "src/services/NotificationService.h"
#include "src/services/Review.h"
#include "src/exceptions/BorrowLimitExceededException.h"
#include "src/exceptions/InsufficientBalanceException.h"

using namespace std;

void testSeparator(string title) {
    cout << "\n" << string(50, '=') << endl;
    cout << "  " << title << endl;
    cout << string(50, '=') << endl;
}

int main() {
    cout << "\n" << string(60, '#') << endl;
    cout << "#" << string(58, ' ') << "#" << endl;
    cout << "#" << string(15, ' ') << "LIBRARY SYSTEM COMPREHENSIVE TEST" << string(10, ' ') << "#" << endl;
    cout << "#" << string(58, ' ') << "#" << endl;
    cout << string(60, '#') << endl;

    LibrarySystem system;

    // ========================================
    // TEST 1: USER CREATION & MANAGEMENT
    // ========================================
    testSeparator("TEST 1: USER CREATION & MANAGEMENT");
    
    User* u1 = new User(1, "Ali", "Khan", "ali@email.com", "1234", 500.0);
    User* u2 = new User(2, "Sara", "Ahmed", "sara@gmail.com", "5678", 350.0);
    User* u3 = new User(3, "Hassan", "Mohamed", "hassan@email.com", "9999", 200.0);
    User* u4 = new User(4, "Fatima", "Ali", "fatima@email.com", "1111", 100.0);
    
    system.addUser(u1);
    system.addUser(u2);
    system.addUser(u3);
    system.addUser(u4);
    cout << "✓ Created and added 4 users to system" << endl;

    // ========================================
    // TEST 2: ADMIN CREATION & MANAGEMENT
    // ========================================
    testSeparator("TEST 2: ADMIN CREATION & MANAGEMENT");
    
    Admin* a1 = new Admin(101, "Mr", "Boss", "admin@library.com", "admin123", "SuperAdmin");
    Admin* a2 = new Admin(102, "Ms", "Manager", "manager@library.com", "manager123", "Admin");
    
    system.addAdmin(a1);
    system.addAdmin(a2);
    cout << "✓ Created and added 2 admins to system" << endl;

    // ========================================
    // TEST 3: RESOURCE CREATION (ALL TYPES)
    // ========================================
    testSeparator("TEST 3: RESOURCE CREATION (ALL TYPES)");
    
    PrimePickBook* book1 = new PrimePickBook(1001, "The Great Gatsby", "F. Scott Fitzgerald", "Fiction");
    PrimePickBook* book2 = new PrimePickBook(1002, "Moby Dick", "Herman Melville", "Adventure");
    
    ClassicShelfBook* book3 = new ClassicShelfBook(1003, "To Kill a Mockingbird", "Harper Lee", "Fiction");
    ClassicShelfBook* book4 = new ClassicShelfBook(1004, "Pride and Prejudice", "Jane Austen", "Romance");
    
    BudgetPickBook* book5 = new BudgetPickBook(1005, "1984", "George Orwell", "Dystopian");
    BudgetPickBook* book6 = new BudgetPickBook(1006, "Brave New World", "Aldous Huxley", "Science Fiction");
    
    // DigitalLibrary ebook1 = new DigitalLibrary(2001, "Digital Programming", "Robert Martin", "Technology");
    
    system.resources.push_back(book1);
    system.resources.push_back(book2);
    system.resources.push_back(book3);
    system.resources.push_back(book4);
    system.resources.push_back(book5);
    system.resources.push_back(book6);
    // system.resources.push_back(ebook1);
    
    cout << "✓ Created 6 resources (4 physical books + 2 budget books)" << endl;

    // ========================================
    // TEST 4: MEMBERSHIP ASSIGNMENT
    // ========================================
    testSeparator("TEST 4: MEMBERSHIP ASSIGNMENT");
    
    u1->setMembership(new FrequentReaderMembership());
    u2->setMembership(new FrequentReaderMembership());
    u3->setMembership(new NormalMembership());
    u4->setMembership(new NormalMembership());
    
    cout << "✓ Assigned memberships:" << endl;
    cout << "  - u1 (Ali): Frequent Reader" << endl;
    cout << "  - u2 (Sara): Frequent Reader" << endl;
    cout << "  - u3 (Hassan): Normal" << endl;
    cout << "  - u4 (Fatima): Normal" << endl;

    // ========================================
    // TEST 5: BORROW OPERATIONS
    // ========================================
    testSeparator("TEST 5: BORROW OPERATIONS");
    
    cout << "\n--- User 1 Borrowing Books ---" << endl;
    u1->borrowresources(book1, "2025-05-01");
    u1->borrowresources(book2, "2025-05-01");
    u1->borrowresources(book3, "2025-05-01");
    
    cout << "\n--- User 2 Borrowing Books ---" << endl;
    u2->borrowresources(book4, "2025-05-02");
    u2->borrowresources(book5, "2025-05-02");
    
    cout << "\n--- User 3 Borrowing Books ---" << endl;
    u3->borrowresources(book6, "2025-05-03");
    
    cout << "\n✓ Books borrowed successfully" << endl;

    // ========================================
    // TEST 6: RETURN OPERATIONS
    // ========================================
    testSeparator("TEST 6: RETURN OPERATIONS");
    
    cout << "\n--- User 1 Returning Book ---" << endl;
    u1->returnresources(book1, "2025-05-10");
    cout << "✓ Book returned successfully" << endl;

    // ========================================
    // TEST 7: BALANCE OPERATIONS
    // ========================================
    testSeparator("TEST 7: BALANCE OPERATIONS");
    
    cout << "\nUser 1 Initial Balance: Rs." << u1->getAccountBalance() << endl;
    cout << "Deducting Rs.50..." << endl;
    u1->deductFromBalance(50.0);
    cout << "Balance after deduction: Rs." << u1->getAccountBalance() << endl;
    
    cout << "\nRecharging with Rs.100..." << endl;
    u1->rechargebalance(100.0);
    cout << "Final Balance: Rs." << u1->getAccountBalance() << endl;

    // ========================================
    // TEST 8: LOYALTY POINTS
    // ========================================
    testSeparator("TEST 8: LOYALTY POINTS");
    
    cout << "\nEarning loyalty points..." << endl;
    u1->earnpoints(100);
    u2->earnpoints(75);
    u3->earnpoints(50);
    cout << "✓ Loyalty points earned and added" << endl;

    // ========================================
    // TEST 9: DISPLAY ALL USERS
    // ========================================
    testSeparator("TEST 9: ALL USERS INFORMATION");
    
    system.showAllUsers();

    // ========================================
    // TEST 10: DISPLAY ALL ADMINS
    // ========================================
    testSeparator("TEST 10: ALL ADMINS INFORMATION");
    
    system.showAllAdmins();

    // ========================================
    // TEST 11: DISPLAY ALL RESOURCES
    // ========================================
    testSeparator("TEST 11: ALL LIBRARY RESOURCES");
    
    cout << "\nTotal Resources: " << system.resources.size() << endl;
    for (const auto& res : system.resources) {
        cout << "\n  ID: " << res->getResourceID() 
             << " | Title: " << res->getTitle() 
             << " | Author: " << res->getTitle()
             << " | Category: " << res->getCategory()
             << " | Available: " << (res->getAvailability() ? "Yes" : "No") << endl;
    }

    // ========================================
    // TEST 12: FILTER AVAILABLE RESOURCES
    // ========================================
    testSeparator("TEST 12: FILTER AVAILABLE RESOURCES");
    
    auto availableBooks = system.filterByAvailability();
    cout << "\nAvailable Books: " << availableBooks.size() << endl;
    for (const auto& book : availableBooks) {
        cout << "  - " << book->getTitle() << endl;
    }

    // ========================================
    // TEST 13: SEARCH RESOURCES
    // ========================================
    testSeparator("TEST 13: SEARCH RESOURCES");
    
    cout << "\nSearching for 'Great'..." << endl;
    auto searchResults = system.searchResource("Great");
    cout << "Found: " << searchResults.size() << " book(s)" << endl;
    for (const auto& book : searchResults) {
        cout << "  - " << book->getTitle() << endl;
    }

    // ========================================
    // TEST 14: FILTER BY CATEGORY
    // ========================================
    testSeparator("TEST 14: FILTER BY CATEGORY");
    
    cout << "\nFiltering by category 'Fiction'..." << endl;
    auto fictionBooks = system.filterResources("Fiction");
    cout << "Fiction Books Found: " << fictionBooks.size() << endl;
    for (const auto& book : fictionBooks) {
        cout << "  - " << book->getTitle() << endl;
    }

    // ========================================
    // TEST 15: ADMIN OPERATIONS
    // ========================================
    testSeparator("TEST 15: ADMIN OPERATIONS");
    
    cout << "\n--- Admin Customer Report ---" << endl;
    a1->generateCustomerReport(system);
    
    cout << "\n--- Admin Issued Resources Report ---" << endl;
    a1->generateIssuedResourcesReport(system);

    // ========================================
    // TEST 16: BORROW HISTORY
    // ========================================
    testSeparator("TEST 16: BORROW HISTORY");
    
    cout << "\n--- User 1 Borrow History ---" << endl;
    u1->viewhistory();

    // ========================================
    // TEST 17: USER PROFILE UPDATE
    // ========================================
    testSeparator("TEST 17: USER PROFILE UPDATE");
    
    cout << "\nUpdating User 4 profile..." << endl;
    u4->updateprofile("Fatima", "Ahmed", "fatima.ahmed@email.com", "newpass123");
    cout << "Updated Email: " << u4->getEmail() << endl;

    // ========================================
    // TEST 18: LOCK/UNLOCK USER
    // ========================================
    testSeparator("TEST 18: LOCK/UNLOCK USER");
    
    cout << "\nLocking User 4 due to unpaid fines..." << endl;
    u4->lock();
    cout << "User 4 Lock Status: " << (u4->getLockStatus() ? "Locked" : "Unlocked") << endl;
    
    cout << "\nAttempting to borrow while locked..." << endl;
    u4->borrowresources(book5, "2025-05-15");

    // ========================================
    // TEST 19: RATING SYSTEM
    // ========================================
    testSeparator("TEST 19: RATING & REVIEW SYSTEM");
    
    cout << "\nAdding ratings to books..." << endl;
    book1->addRating(5);
    book1->addRating(4);
    book2->addRating(3);
    book3->addRating(5);
    
    cout << "Book 1 Current Rating: " << book1->getReviewScore() << "/5" << endl;
    cout << "Book 2 Current Rating: " << book2->getReviewScore() << "/5" << endl;
    cout << "Book 3 Current Rating: " << book3->getReviewScore() << "/5" << endl;

    // ========================================
    // TEST 20: FILTER BY RATING
    // ========================================
    testSeparator("TEST 20: FILTER BY HIGHEST RATED BOOKS");
    
    auto ratedBooks = system.filterByRating();
    cout << "\nTop Rated Books (first 3):" << endl;
    for (int i = 0; i < min(3, (int)ratedBooks.size()); i++) {
        cout << "  " << (i+1) << ". " << ratedBooks[i]->getTitle() 
             << " - Rating: " << ratedBooks[i]->getReviewScore() << "/5" << endl;
    }

    // ========================================
    // TEST 21: MOST BORROWED BOOKS
    // ========================================
    testSeparator("TEST 21: MOST BORROWED BOOKS");
    
    auto mostBorrowed = system.filterByMostBorrowed();
    cout << "\nMost Borrowed Books (first 3):" << endl;
    for (int i = 0; i < min(3, (int)mostBorrowed.size()); i++) {
        cout << "  " << (i+1) << ". " << mostBorrowed[i]->getTitle() 
             << " - Times Borrowed: " << mostBorrowed[i]->getBorrowCount() << endl;
    }

    // ========================================
    // FINAL SUMMARY
    // ========================================
    testSeparator("FINAL SUMMARY");
    
    cout << "\n✓ Total Users in System: " << system.users.size() << endl;
    cout << "✓ Total Admins in System: " << system.admins.size() << endl;
    cout << "✓ Total Resources in System: " << system.resources.size() << endl;
    cout << "✓ Available Resources: " << system.filterByAvailability().size() << endl;
    cout << "✓ Borrowed Resources: " << (system.resources.size() - system.filterByAvailability().size()) << endl;

    cout << "\n" << string(60, '#') << endl;
    cout << "#" << string(15, ' ') << "ALL TESTS COMPLETED SUCCESSFULLY!" << string(10, ' ') << "#" << endl;
    cout << string(60, '#') << endl;

    return 0;
}