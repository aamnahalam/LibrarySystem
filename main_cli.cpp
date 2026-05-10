#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <iomanip>
#include <ctime>
#include <sstream>
#include "src/core/LibrarySystem.h"
#include "src/users/user.h"
#include "src/users/admin.h"
#include "src/resources/PrimePickBook.h"
#include "src/resources/ClassicShelfBook.h"
#include "src/resources/BudgetPickBook.h"
#include "src/services/Review.h"

using namespace std;

LibrarySystem* globalSystem = nullptr;
User* currentUser = nullptr;
Admin* currentAdmin = nullptr;

void clearScreen() {
    system("cls");
}

void pause() {
    cout << "\nPress Enter to continue...";
    string dummy;
    getline(cin, dummy);
}

// Helper function to get current date in YYYY-MM-DD format
string getCurrentDate() {
    time_t now = time(0);
    struct tm* timeinfo = localtime(&now);
    char buffer[11];
    strftime(buffer, 11, "%Y-%m-%d", timeinfo);
    return string(buffer);
}

// Helper function to calculate due date (7 days from borrow date)
string getDueDate(string borrowDate) {
    int year, month, day;
    sscanf(borrowDate.c_str(), "%d-%d-%d", &year, &month, &day);
    
    time_t t = time(0);
    struct tm* timeinfo = localtime(&t);
    timeinfo->tm_year = year - 1900;
    timeinfo->tm_mon = month - 1;
    timeinfo->tm_mday = day + 7;  // Add 7 days
    mktime(timeinfo);
    
    char buffer[11];
    strftime(buffer, 11, "%Y-%m-%d", timeinfo);
    return string(buffer);
}

void submitReview(Resource* book) {
    clearScreen();
    cout << "============================================================\n";
    cout << "                    SUBMIT REVIEW                           \n";
    cout << "============================================================\n\n";
    cout << "Book: " << book->getTitle() << "\n";
    cout << "Author: " << book->getAuthor() << "\n";
    cout << string(60, '-') << "\n\n";
    
    int rating = 0;
    string reviewText;
    bool validRating = false;
    
    while (!validRating) {
        cout << "Please rate the book (1-5 stars): ";
        if (cin >> rating) {
            if (rating >= 1 && rating <= 5) {
                validRating = true;
            } else {
                cout << "[ERROR] Invalid rating. Must be between 1 and 5.\n";
                cin.clear();
                cin.ignore(10000, '\n');
            }
        } else {
            cout << "[ERROR] Please enter a valid number.\n";
            cin.clear();
            cin.ignore(10000, '\n');
        }
    }
    cin.ignore(10000, '\n');
    
    cout << "\nPlease write your review (or press Enter to skip): ";
    getline(cin, reviewText);
    
    if (reviewText.empty()) {
        cout << "[INFO] No review text provided.\n";
        reviewText = "No text provided.";
    }
    
    // Create and add review
    Review* newReview = new Review(rating, reviewText, currentUser);
    book->addReview(newReview);
    globalSystem->saveData();
    
    cout << "\n[SUCCESS] Review submitted!\n";
    cout << "Current Book Rating: ";
    double avgRating = book->getAverageRating();
    for (int i = 0; i < (int)avgRating; i++) cout << "*";
    cout << " (" << fixed << setprecision(1) << avgRating << "/5.0) - " 
         << book->getReviewCount() << " reviews\n";
}

void viewBookReviews() {
    clearScreen();
    cout << "============================================================\n";
    cout << "                    VIEW BOOK REVIEWS                        \n";
    cout << "============================================================\n\n";
    
    if (globalSystem->resources.empty()) {
        cout << "No books available.\n";
        pause();
        return;
    }
    
    cout << "Available Books:\n";
    cout << left << setw(6) << "ID" << setw(30) << "Title" << setw(15) << "Rating\n";
    cout << string(60, '-') << "\n";
    
    for (size_t i = 0; i < globalSystem->resources.size(); ++i) {
        Resource* r = globalSystem->resources[i];
        cout << left << setw(6) << r->getResourceID() 
             << setw(30) << r->getTitle().substr(0, 28);
        
        // Display star rating
        for (int j = 0; j < (int)r->getAverageRating(); j++) cout << "*";
        cout << " (" << fixed << setprecision(1) << r->getAverageRating() << "/5.0)\n";
    }
    
    cout << "\nEnter Book ID to view reviews (0 to go back): ";
    int bookId;
    cin >> bookId;
    cin.ignore();
    
    if (bookId == 0) return;
    
    Resource* selectedBook = nullptr;
    for (auto& r : globalSystem->resources) {
        if (r->getResourceID() == bookId) {
            selectedBook = r;
            break;
        }
    }
    
    if (!selectedBook) {
        cout << "\n[ERROR] Book not found.\n";
        pause();
        return;
    }
    
    selectedBook->displayReviews();
    pause();
}

void displayMainMenu() {
    clearScreen();
    cout << "============================================================\n";
    cout << "             LIBRARY MANAGEMENT SYSTEM v2.0                  \n";
    cout << "                     CLI VERSION                             \n";
    cout << "============================================================\n\n";
    cout << "1. User Login\n";
    cout << "2. Admin Login\n";
    cout << "3. Register New User\n";
    cout << "4. Exit\n";
    cout << "\nSelect option (1-4): ";
}

// ==================== USER FEATURES ====================


void userLogin() {
    clearScreen();
    cout << "============================================================\n";
    cout << "                    USER LOGIN                               \n";
    cout << "============================================================\n\n";
    
    cout << "Tip: Register a new account or use existing credentials.\n\n";
    cout << string(60, '-') << "\n\n";
    
    string email, password;
    cout << "Email: ";
    getline(cin, email);
    cout << "Password: ";
    getline(cin, password);
    
    if (globalSystem->authenticate(email, password)) {
        currentUser = globalSystem->getCurrentUser();
        cout << "\n[SUCCESS] Login successful!\n";
        cout << "Welcome " << currentUser->getFullName() << "\n";
        pause();
    } else {
        cout << "\n[ERROR] Invalid credentials. Please try again or register.\n";
        pause();
    }
}

void registerUser() {
    clearScreen();
    cout << "============================================================\n";
    cout << "                  NEW USER REGISTRATION                      \n";
    cout << "============================================================\n\n";
    
    string firstName, lastName, email, password;
    double balance = 0;
    
    cout << "First Name: ";
    getline(cin, firstName);
    cout << "Last Name: ";
    getline(cin, lastName);
    cout << "Email: ";
    getline(cin, email);
    cout << "Password (min 6 chars): ";
    getline(cin, password);
    
    // Validate and input balance with error handling
    bool validBalance = false;
    while (!validBalance) {
        cout << "Initial Balance ($): ";
        if (cin >> balance) {
            if (balance >= 0) {
                validBalance = true;
            } else {
                cout << "Balance must be non-negative.\n";
                cin.clear();
                cin.ignore(10000, '\n');
            }
        } else {
            cout << "Please enter a valid number.\n";
            cin.clear();
            cin.ignore(10000, '\n');
        }
    }
    cin.ignore(10000, '\n');
    
    if (password.length() < 6) {
        cout << "\n[ERROR] Password must be at least 6 characters.\n";
        pause();
        return;
    }
    
    for (auto& user : globalSystem->users) {
        if (user->getEmail() == email) {
            cout << "\n[ERROR] Email already registered.\n";
            pause();
            return;
        }
    }
    
    for (auto& admin : globalSystem->admins) {
        if (admin->getEmail() == email) {
            cout << "\n[ERROR] Email already registered.\n";
            pause();
            return;
        }
    }
    
    try {
        globalSystem->registerUser(firstName, lastName, email, password, balance);
        cout << "\nRegistration successful!\n";
        cout << "You can now login with your credentials.\n";
    } catch (...) {
        cout << "\n[ERROR] Error during registration.\n";
    }
    pause();
}

void displayUserMenu() {
    cout << "\n" << string(50, '=') << "\n";
    cout << "  Logged in as: " << currentUser->getFullName() << "\n";
    cout << "  Balance: $" << fixed << setprecision(2) << currentUser->getAccountBalance() 
         << " | Points: " << currentUser->getLoyaltyPoints() << "\n";
    cout << string(50, '=') << "\n\n";
    cout << "1. View Available Books\n";
    cout << "2. View Book Details\n";
    cout << "3. View Books in My Favorite Categories\n";
    cout << "4. Borrow a Book\n";
    cout << "5. Return a Book\n";
    cout << "6. View My Borrowing History\n";
    cout << "7. View My Profile\n";
    cout << "8. Update Profile\n";
    cout << "9. View Membership Details\n";
    cout << "10. Change Membership Tier\n";
    cout << "11. View Loyalty Points Summary\n";
    cout << "12. Redeem Loyalty Points for Discount\n";
    cout << "13. Redeem Points for Fine Waiver\n";
    cout << "14. Upgrade Membership with Loyalty Points\n";
    cout << "15. Recharge Account Balance\n";
    cout << "16. View Book Reviews\n";
    cout << "17. Logout\n";
    cout << "\nSelect option (1-17): ";
}

void viewBooks() {
    clearScreen();
    cout << "---------- AVAILABLE BOOKS ----------\n\n";
    
    if (globalSystem->resources.empty()) {
        cout << "No books available.\n";
        pause();
        return;
    }
    
    cout << left << setw(6) << "ID" << setw(25) << "Title" << setw(18) << "Author" 
         << setw(12) << "Category" << setw(10) << "Status" << setw(8) << "Rating\n";
    cout << string(80, '-') << "\n";
    
    for (size_t i = 0; i < globalSystem->resources.size(); ++i) {
        Resource* r = globalSystem->resources[i];
        string typeName = "Unknown";
        if (dynamic_cast<PrimePickBook*>(r)) typeName = "PrimePick";
        else if (dynamic_cast<ClassicShelfBook*>(r)) typeName = "ClassicShelf";
        else if (dynamic_cast<BudgetPickBook*>(r)) typeName = "BudgetPick";
        
        cout << left << setw(6) << r->getResourceID() 
             << setw(25) << r->getTitle().substr(0, 24)
             << setw(18) << r->getAuthor().substr(0, 17)
             << setw(12) << r->getCategory().substr(0, 11)
             << setw(10) << (r->getAvailability() ? "Available" : "Borrowed")
             << setw(8) << fixed << setprecision(1) << r->getRating() << "\n";
    }
    pause();
}

void viewBookDetails() {
    clearScreen();
    cout << "========== VIEW BOOK DETAILS ==========\n\n";
    
    if (globalSystem->resources.empty()) {
        cout << "No books available.\n";
        pause();
        return;
    }
    
    cout << "Available Books:\n";
    cout << left << setw(6) << "ID" << setw(30) << "Title" << setw(15) << "Author\n";
    cout << string(52, '-') << "\n";
    
    for (size_t i = 0; i < globalSystem->resources.size(); ++i) {
        Resource* r = globalSystem->resources[i];
        cout << left << setw(6) << r->getResourceID() 
             << setw(30) << r->getTitle().substr(0, 29)
             << setw(15) << r->getAuthor().substr(0, 14) << "\n";
    }
    
    cout << "\nEnter Book ID to view details (0 to cancel): ";
    int bookId;
    cin >> bookId;
    cin.ignore();
    
    if (bookId == 0) {
        return;
    }
    
    Resource* book = nullptr;
    for (auto& r : globalSystem->resources) {
        if (r->getResourceID() == bookId) {
            book = r;
            break;
        }
    }
    
    if (!book) {
        cout << "\n[ERROR] Book not found.\n";
        pause();
        return;
    }
    
    clearScreen();
    cout << "========== DETAILED BOOK INFORMATION ==========\n\n";
    book->displayDetails();
    
    cout << "\n--- Additional Information ---\n";
    cout << "Category: " << book->getCategory() << "\n";
    cout << "Rating: " << fixed << setprecision(1) << book->getRating() << "/5.0\n";
    cout << "Borrow Count: " << book->getBorrowCount() << "\n";
    cout << "Status: " << (book->getAvailability() ? "Available" : "Currently Borrowed") << "\n";
    cout << "Fine Rate: Rs." << fixed << setprecision(1) << book->getFineRate() << " per day\n";
    
    if (book->getIsNewArrival()) {
        cout << "[NEW ARRIVAL] This is a recently added book!\n";
    }
    
    pause();
}

void viewBooksByFavoriteCategories() {
    clearScreen();
    cout << "========== BOOKS IN YOUR FAVORITE CATEGORIES ==========\n\n";
    
    // Get user's favorite categories (from borrowHistory analysis)
    set<string> favoriteCategories;
    const vector<BorrowRecord>& history = currentUser->getBorrowHistory();
    
    // Analyze borrowed books to find favorite categories
    for (const auto& record : history) {
        for (auto& r : globalSystem->resources) {
            if (r->getTitle() == record.getResourceName()) {
                favoriteCategories.insert(r->getCategory());
                break;
            }
        }
    }
    
    if (favoriteCategories.empty()) {
        cout << "[INFO] You haven't borrowed any books yet.\n";
        cout << "Start borrowing books to discover your favorite categories!\n";
        pause();
        return;
    }
    
    cout << "Your Favorite Categories: ";
    for (const auto& cat : favoriteCategories) {
        cout << cat << " | ";
    }
    cout << "\n\n";
    
    // Filter books by favorite categories
    vector<Resource*> filteredBooks;
    for (auto& r : globalSystem->resources) {
        if (favoriteCategories.count(r->getCategory()) > 0) {
            filteredBooks.push_back(r);
        }
    }
    
    if (filteredBooks.empty()) {
        cout << "No books available in your favorite categories.\n";
        pause();
        return;
    }
    
    // Sort by rating (highest first)
    sort(filteredBooks.begin(), filteredBooks.end(), 
         [](Resource* a, Resource* b) { return a->getRating() > b->getRating(); });
    
    cout << left << setw(6) << "ID" << setw(25) << "Title" << setw(18) << "Author" 
         << setw(12) << "Category" << setw(10) << "Status" << setw(8) << "Rating\n";
    cout << string(80, '-') << "\n";
    
    for (auto& r : filteredBooks) {
        cout << left << setw(6) << r->getResourceID() 
             << setw(25) << r->getTitle().substr(0, 24)
             << setw(18) << r->getAuthor().substr(0, 17)
             << setw(12) << r->getCategory().substr(0, 11)
             << setw(10) << (r->getAvailability() ? "Available" : "Borrowed")
             << setw(8) << fixed << setprecision(1) << r->getRating() << "\n";
    }
    
    cout << "\nTotal: " << filteredBooks.size() << " books in your favorite categories\n";
    pause();
}

void borrowBook() {
    clearScreen();
    cout << "---------- BORROW A BOOK ----------\n\n";
    
    // Display available books
    cout << "Available Books:\n\n";
    if (globalSystem->resources.empty()) {
        cout << "No books available.\n";
        pause();
        return;
    }
    
    cout << left << setw(6) << "ID" << setw(25) << "Title" << setw(18) << "Author" 
         << setw(12) << "Category" << setw(8) << "Rating\n";
    cout << string(80, '-') << "\n";
    
    bool hasAvailable = false;
    for (auto& r : globalSystem->resources) {
        if (r->getAvailability()) {
            hasAvailable = true;
            cout << left << setw(6) << r->getResourceID() 
                 << setw(25) << r->getTitle().substr(0, 24)
                 << setw(18) << r->getAuthor().substr(0, 17)
                 << setw(12) << r->getCategory().substr(0, 11)
                 << setw(8) << fixed << setprecision(1) << r->getRating() << "\n";
        }
    }
    
    if (!hasAvailable) {
        cout << "No books currently available.\n";
        pause();
        return;
    }
    
    cout << "\n";
    int bookId;
    cout << "Enter Book ID to borrow: ";
    cin >> bookId;
    cin.ignore();
    
    Resource* book = nullptr;
    for (auto& r : globalSystem->resources) {
        if (r->getResourceID() == bookId) {
            book = r;
            break;
        }
    }
    
    if (!book) {
        cout << "\n[ERROR] Book not found.\n";
        pause();
        return;
    }
    
    if (!book->getAvailability()) {
        cout << "\n[ERROR] Book is not available.\n";
        pause();
        return;
    }
    
    try {
        string borrowDate = getCurrentDate();
        string dueDate = getDueDate(borrowDate);
        
        // Check specific error conditions
        if (currentUser->getLockStatus()) {
            cout << "\n[ERROR] Access Denied: Your account is locked.\n";
        } else if (currentUser->getAccountBalance() < 0) {
            cout << "\n[ERROR] Access Denied: You have unpaid fines. Please recharge your account.\n";
        } else if (currentUser->borrowresources(book, borrowDate)) {
            globalSystem->saveData();
            cout << "\nBook borrowed successfully!\n";
            cout << "Title: " << book->getTitle() << "\n";
            cout << "Author: " << book->getAuthor() << "\n";
            cout << "Borrow Date: " << borrowDate << "\n";
            cout << "Due Date: " << dueDate << " (7 days)\n";
        } else {
            cout << "\n[ERROR] Could not borrow book. You may have exceeded your borrowing limit.\n";
        }
    } catch (const exception& e) {
        cout << "\n[ERROR] Error: " << e.what() << "\n";
    }
    pause();
}

void returnBook() {
    clearScreen();
    cout << "---------- RETURN A BOOK ----------\n\n";
    
    // Display currently borrowed books
    cout << "Your Borrowed Books:\n\n";
    const vector<BorrowRecord>& history = currentUser->getBorrowHistory();
    
    bool hasBorrowed = false;
    cout << left << setw(6) << "ID" << setw(25) << "Title" << setw(18) << "Author" 
         << setw(12) << "Due Date\n";
    cout << string(65, '-') << "\n";
    
    for (const auto& record : history) {
        if (!record.getReturnStatus()) {  // Not yet returned
            hasBorrowed = true;
            // Find the resource to get the ID
            for (auto& r : globalSystem->resources) {
                if (r->getTitle() == record.getResourceName()) {
                    cout << left << setw(6) << r->getResourceID() 
                         << setw(25) << record.getResourceName().substr(0, 24)
                         << setw(18) << r->getAuthor().substr(0, 17)
                         << setw(12) << record.getDueDate() << "\n";
                    break;
                }
            }
        }
    }
    
    if (!hasBorrowed) {
        cout << "You have no borrowed books to return.\n";
        pause();
        return;
    }
    
    cout << "\n";
    int bookId;
    cout << "Enter Book ID to return: ";
    cin >> bookId;
    cin.ignore();
    
    Resource* book = nullptr;
    for (auto& r : globalSystem->resources) {
        if (r->getResourceID() == bookId) {
            book = r;
            break;
        }
    }
    
    if (!book) {
        cout << "\n[ERROR] Book not found.\n";
        pause();
        return;
    }
    
    try {
        string returnDate = getCurrentDate();
        
        // Test mode: allow custom return date for fine testing
        cout << "\n[TEST MODE] Use custom return date to test fine calculation?\n";
        cout << "Enter 'y' for yes, or press Enter to use today's date: ";
        string testInput;
        getline(cin, testInput);
        
        if (testInput == "y" || testInput == "Y") {
            cout << "Enter return date (YYYY-MM-DD) for testing: ";
            getline(cin, returnDate);
            cout << "[TEST] Using custom return date: " << returnDate << "\n";
        }
        
    
        cout << "Current Balance: Rs." << fixed << setprecision(2) << currentUser->getAccountBalance() << "\n";
        bool waiverWasActive = currentUser->hasFineWaiverActive();
        cout << "Fine Waiver Active: " << (waiverWasActive ? "YES" : "NO") << "\n";
        
        double fine = currentUser->returnresources(book, returnDate);
        
        if (fine < 0) {
            cout << "\n[ERROR] Book not found in your borrowed list or invalid return date.\n";
        } else {
            globalSystem->saveData();
            cout << "\nBook returned successfully!\n";
            cout << "Title: " << book->getTitle() << "\n";
            cout << "Return Date: " << returnDate << "\n";
            
            if (fine > 0) {
                cout << "\n[FINE CHARGED] Rs." << fixed << setprecision(2) << fine << "\n";
                cout << "New Balance: Rs." << fixed << setprecision(2) << currentUser->getAccountBalance() << "\n";
                if (currentUser->getAccountBalance() < 0) {
                    cout << "WARNING: Your balance is NEGATIVE! You have unpaid fines.\n";
                } else {
                    cout << "Balance is sufficient.\n";
                }
            } else if (waiverWasActive) {
                cout << "\n*** FINE WAIVER APPLIED ***\n";
                cout << "You would have been charged a fine, but your waiver covered it!\n";
                cout << "Balance: Rs." << fixed << setprecision(2) << currentUser->getAccountBalance() << "\n";
            } else {
                cout << "No fines incurred.\n";
                cout << "[LOYALTY POINTS] +10 points earned for returning on time!\n";
                cout << "Total Loyalty Points: " << currentUser->getLoyaltyPoints() << "\n";
            }
            
            // Ask if user wants to submit a review
            cout << "\nWould you like to submit a review and rating? (y/n): ";
            char choice;
            cin >> choice;
            cin.ignore();
            
            if (choice == 'y' || choice == 'Y') {
                submitReview(book);
            }
            
            // Ask if user wants to view reviews for this book
            cout << "\nWould you like to view all reviews for this book? (y/n): ";
            cin >> choice;
            cin.ignore();
            
            if (choice == 'y' || choice == 'Y') {
                book->displayReviews();
            }
        }
    } catch (const exception& e) {
        cout << "\n[ERROR] Error: " << e.what() << "\n";
    }
    pause();
}

void viewBorrowingHistory() {
    clearScreen();
    cout << "---------- BORROWING HISTORY ----------\n\n";
    
    const vector<BorrowRecord>& history = currentUser->getBorrowHistory();
    
    if (history.empty()) {
        cout << "No borrow history yet.\n";
        pause();
        return;
    }
    
    for (const auto& record : history) {
        record.showRecord();
        cout << "\n";
    }
    pause();
}

void viewProfile() {
    clearScreen();
    cout << "---------- USER PROFILE ----------\n\n";
    cout << "Name: " << currentUser->getFullName() << "\n";
    cout << "Email: " << currentUser->getEmail() << "\n";
    cout << "ID: " << currentUser->getID() << "\n";
    cout << "Account Balance: $" << fixed << setprecision(2) << currentUser->getAccountBalance() << "\n";
    cout << "Membership: " << currentUser->getMembershipName() << "\n";
    cout << "Loyalty Points: " << currentUser->getLoyaltyPoints() << "\n";
    pause();
}

void updateProfile() {
    clearScreen();
    cout << "---------- UPDATE PROFILE ----------\n\n";
    
    string currentName = currentUser->getFullName();
    size_t spacePos = currentName.find(' ');
    string currentFirstName = currentName.substr(0, spacePos);
    string currentLastName = (spacePos != string::npos) ? currentName.substr(spacePos + 1) : "";
    
    string firstName, lastName, email, password;
    
    cout << "Leave blank to keep current values.\n\n";
    cout << "First Name [" << currentFirstName << "]: ";
    getline(cin, firstName);
    if (firstName.empty()) firstName = currentFirstName;
    
    cout << "Last Name [" << currentLastName << "]: ";
    getline(cin, lastName);
    if (lastName.empty()) lastName = currentLastName;
    
    cout << "Email [" << currentUser->getEmail() << "]: ";
    getline(cin, email);
    if (email.empty()) email = currentUser->getEmail();
    
    cout << "Password [unchanged]: ";
    getline(cin, password);
    if (password.empty()) password = currentUser->getPassword();
    
    try {
        currentUser->updateprofile(firstName, lastName, email, password);
        globalSystem->saveData();
        cout << "\nProfile updated successfully!\n";
    } catch (const exception& e) {
        cout << "\n[ERROR] Error: " << e.what() << "\n";
    }
    pause();
}

void redeemLoyaltyPoints() {
    clearScreen();
    cout << "---------- REDEEM LOYALTY POINTS ----------\n\n";
    
    cout << "Current Loyalty Points: " << currentUser->getLoyaltyPoints() << "\n";
    cout << "Conversion Rate: 100 points = Rs.50 discount\n\n";
    
    int pointsToRedeem = 0;
    bool validInput = false;
    
    while (!validInput) {
        cout << "Points to redeem (must be multiple of 100): ";
        if (cin >> pointsToRedeem) {
            if (pointsToRedeem > 0 && pointsToRedeem % 100 == 0) {
                validInput = true;
            } else if (pointsToRedeem <= 0) {
                cout << "[ERROR] Points must be greater than zero.\n";
                cin.clear();
                cin.ignore(10000, '\n');
            } else {
                cout << "[ERROR] Points must be in multiples of 100.\n";
                cin.clear();
                cin.ignore(10000, '\n');
            }
        } else {
            cout << "[ERROR] Please enter a valid number.\n";
            cin.clear();
            cin.ignore(10000, '\n');
        }
    }
    cin.ignore(10000, '\n');
    
    try {
        if (currentUser->redeemPointsForDiscount(pointsToRedeem)) {
            globalSystem->saveData();
            double discount = (pointsToRedeem / 100.0) * 50;
            cout << "\n[SUCCESS] Redeemed " << pointsToRedeem << " points for Rs." << fixed << setprecision(2) << discount << " discount!\n";
            cout << "Amount added to your balance!\n";
            cout << "Remaining Points: " << currentUser->getLoyaltyPoints() << "\n";
            cout << "New Balance: Rs." << fixed << setprecision(2) << currentUser->getAccountBalance() << "\n";
        } else {
            cout << "\n[ERROR] Insufficient loyalty points.\n";
            cout << "You have: " << currentUser->getLoyaltyPoints() << " points\n";
        }
    } catch (const exception& e) {
        cout << "\n[ERROR] Error: " << e.what() << "\n";
    }
    pause();
}

void rechargeBalance() {
    clearScreen();
    cout << "---------- RECHARGE ACCOUNT BALANCE ----------\n\n";
    
    cout << "Current Balance: $" << fixed << setprecision(2) << currentUser->getAccountBalance() << "\n\n";
    
    double amount = 0;
    bool validAmount = false;
    
    while (!validAmount) {
        cout << "Amount to add: $";
        if (cin >> amount) {
            if (amount > 0) {
                validAmount = true;
            } else {
                cout << "[ERROR] Amount must be greater than zero.\n";
                cin.clear();
                cin.ignore(10000, '\n');
            }
        } else {
            cout << "[ERROR] Please enter a valid number.\n";
            cin.clear();
            cin.ignore(10000, '\n');
        }
    }
    cin.ignore(10000, '\n');
    
    try {
        currentUser->rechargebalance(amount);
        globalSystem->saveData();
        cout << "\nBalance recharged successfully!\n";
        cout << "New Balance: $" << fixed << setprecision(2) << currentUser->getAccountBalance() << "\n";
    } catch (const exception& e) {
        cout << "\n[ERROR] Error: " << e.what() << "\n";
    }
    pause();
}

void viewMembershipDetails() {
    clearScreen();
    cout << "---------- MEMBERSHIP DETAILS ----------\n\n";
    
    try {
        currentUser->displayMembershipDetails();
    } catch (const exception& e) {
        cout << "[ERROR] Error: " << e.what() << "\n";
    }
    pause();
}

void changeMembershipTier() {
    clearScreen();
    cout << "---------- CHANGE MEMBERSHIP TIER ----------\n\n";
    
    try {
        currentUser->showMembershipOptions();
        cout << "\n";
        
        int tier;
        cout << "Select membership tier (1-3): ";
        cin >> tier;
        cin.ignore();
        
        if (tier < 1 || tier > 3) {
            cout << "\n[ERROR] Invalid choice. Please select 1, 2, or 3.\n";
            pause();
            return;
        }
        
        string notice = currentUser->getMembershipChangeNotice(tier);
        cout << "\n" << notice << "\n\n";
        
        if (notice.find("Warning") != string::npos || notice.find("costs") != string::npos) {
            cout << "Do you want to proceed? (y/n): ";
            char confirm;
            cin >> confirm;
            cin.ignore();
            
            if (confirm != 'y' && confirm != 'Y') {
                cout << "\nMembership change cancelled.\n";
                pause();
                return;
            }
        }
        
        if (currentUser->changeMembershipTier(tier, true)) {
            globalSystem->saveData();
            cout << "\nMembership changed successfully!\n";
        } else {
            cout << "\n[ERROR] Could not change membership tier.\n";
        }
    } catch (const exception& e) {
        cout << "\n[ERROR] Error: " << e.what() << "\n";
    }
    pause();
}

void viewLoyaltySummary() {
    clearScreen();
    cout << "---------- LOYALTY POINTS SUMMARY ----------\n\n";
    
    try {
        currentUser->displayLoyaltySummary();
    } catch (const exception& e) {
        cout << "[ERROR] Error: " << e.what() << "\n";
    }
    pause();
}

void redeemFineFreePass() {
    clearScreen();
    cout << "---------- REDEEM FINE-FREE PASS ----------\n\n";
    
    cout << "Current Loyalty Points: " << currentUser->getLoyaltyPoints() << "\n";
    cout << "Cost: 100 points\n";
    cout << "Benefit: Your next overdue fine will be waived!\n\n";
    
    try {
        if (currentUser->redeemPointsForFineFreePass()) {
            globalSystem->saveData();
            cout << "\n[SUCCESS] Fine-free pass redeemed successfully!\n";
            cout << "Redeemed 100 loyalty points!\n";
            cout << "Your next overdue fine will be waived!\n";
            cout << "Remaining Points: " << currentUser->getLoyaltyPoints() << "\n";
        } else {
            cout << "\n[ERROR] Insufficient loyalty points for fine waiver.\n";
            cout << "You have: " << currentUser->getLoyaltyPoints() << " points (Need: 100)\n";
        }
    } catch (const exception& e) {
        cout << "\n[ERROR] Error: " << e.what() << "\n";
    }
    pause();
}

void upgradeMembershipWithPoints() {
    clearScreen();
    cout << "========== UPGRADE MEMBERSHIP WITH LOYALTY POINTS ==========\n\n";
    
    cout << "Current Loyalty Points: " << currentUser->getLoyaltyPoints() << "\n";
    cout << "Current Membership: " << currentUser->getMembershipName() << "\n";
    cout << "Cost: 1000 points\n";
    cout << "Benefit: Upgrade to Deluxe membership!\n";
    cout << "  - Borrow up to 10 books\n";
    cout << "  - 50% fine discount\n";
    cout << "  - Free fine waiver pass\n";
    cout << "  - Priority queue access\n\n";
    
    if (currentUser->getLoyaltyPoints() < 1000) {
        cout << "[ERROR] You need 1000 loyalty points to upgrade.\n";
        cout << "You have: " << currentUser->getLoyaltyPoints() << " points (Need: " 
             << (1000 - currentUser->getLoyaltyPoints()) << " more)\n";
        pause();
        return;
    }
    
    cout << "Do you want to upgrade? (y/n): ";
    char confirm;
    cin >> confirm;
    cin.ignore();
    
    if (confirm == 'y' || confirm == 'Y') {
        try {
            if (currentUser->checkAndUpgradeMembership()) {
                globalSystem->saveData();
                cout << "\n[SUCCESS] Membership upgraded successfully!\n";
                cout << "Remaining Loyalty Points: " << currentUser->getLoyaltyPoints() << "\n";
            } else {
                cout << "\n[INFO] Membership upgrade not available at this time.\n";
                cout << "You may already have the highest tier or meet upgrade conditions.\n";
            }
        } catch (const exception& e) {
            cout << "\n[ERROR] Error during upgrade: " << e.what() << "\n";
        }
    } else {
        cout << "\nUpgrade cancelled.\n";
    }
    pause();
}

void userLogout() {
    currentUser = nullptr;
    cout << "\nLogged out successfully.\n";
    pause();
}


// ==================== ADMIN FEATURES ====================

void adminLogin() {
    clearScreen();
    cout << "============================================================\n";
    cout << "                    ADMIN LOGIN                              \n";
    cout << "============================================================\n\n";
    
    cout << ">> Demo Admin Credentials:\n";
    cout << "   Email: admin@library.com        | Password: admin123\n";
    cout << "   Email: manager@library.com      | Password: manager123\n\n";
    cout << string(60, '-') << "\n\n";
    
    string email, password;
    cout << "Email: ";
    getline(cin, email);
    cout << "Password: ";
    getline(cin, password);
    
    bool found = false;
    for (auto& admin : globalSystem->admins) {
        if (admin->getEmail() == email && admin->getPassword() == password) {
            currentAdmin = admin;
            found = true;
            break;
        }
    }
    
    if (found) {
        cout << "\nAdmin login successful!\n";
        cout << "Welcome " << currentAdmin->getFullName() << " (" << currentAdmin->getAccessLevel() << ")\n";
        pause();
    } else {
        cout << "\n[ERROR] Invalid admin credentials.\n";
        pause();
    }
}

void displayAdminMenu() {
    cout << "\n" << string(44, '=') << "\n";
    cout << "  Admin: " << currentAdmin->getFullName() << "\n";
    cout << "  Level: " << currentAdmin->getAccessLevel() << "\n";
    cout << string(44, '=') << "\n\n";
    cout << "1. Add New Book\n";
    cout << "2. Remove Book\n";
    cout << "3. Update Book\n";
    cout << "4. View All Books\n";
    cout << "5. View All Users\n";
    cout << "6. Add New User\n";
    cout << "7. Lock/Unlock User\n";
    cout << "8. View Borrow History\n";
    cout << "9. Add New Admin\n";
    cout << "10. View Reports\n";
    cout << "11. Update User Membership\n";
    cout << "12. Logout\n";
    cout << "\nSelect option (1-12): ";
}

void addBook() {
    clearScreen();
    cout << "---------- ADD NEW BOOK ----------\n\n";
    
    string title, author, category, typeStr;
    int bookCount = 0;
    for (auto& r : globalSystem->resources) bookCount++;
    
    cout << "Title: ";
    getline(cin, title);
    cout << "Author: ";
    getline(cin, author);
    cout << "Category: ";
    getline(cin, category);
    cout << "Book Type (1=PrimePick, 2=ClassicShelf, 3=BudgetPick): ";
    getline(cin, typeStr);
    
    try {
        int newId = 5001 + bookCount;
        Resource* newBook = nullptr;
        
        if (typeStr == "1") {
            newBook = new PrimePickBook(newId, title, author, category);
        } else if (typeStr == "2") {
            newBook = new ClassicShelfBook(newId, title, author, category);
        } else if (typeStr == "3") {
            newBook = new BudgetPickBook(newId, title, author, category);
        } else {
            cout << "\n[ERROR] Invalid book type.\n";
            pause();
            return;
        }
        
        globalSystem->resources.push_back(newBook);
        globalSystem->saveData();
        
        cout << "\nBook added successfully!\n";
        cout << "ID: " << newId << "\n";
        cout << "Title: " << title << "\n";
    } catch (...) {
        cout << "\n[ERROR] Error adding book.\n";
    }
    pause();
}

// Forward declaration: used before its definition in this file
void viewAllBooksAdmin();

void removeBook() {
    clearScreen();
    cout << "---------- REMOVE BOOK ----------\n\n";
    
    viewAllBooksAdmin();
    
    int bookId;
    cout << "Enter Book ID to remove: ";
    cin >> bookId;
    cin.ignore();
    
    try {
        if (currentAdmin->removeResources(bookId, *globalSystem)) {
            globalSystem->saveData();
            cout << "\nBook removed successfully!\n";
        } else {
            cout << "\n[ERROR] Book not found.\n";
        }
    } catch (const exception& e) {
        cout << "\n[ERROR] Error: " << e.what() << "\n";
    }
    pause();
}

void updateBook() {
    clearScreen();
    cout << "---------- UPDATE BOOK ----------\n\n";
    
    viewAllBooksAdmin();
    
    int bookId;
    cout << "Enter Book ID to update (mark as available): ";
    cin >> bookId;
    cin.ignore();
    
    try {
        if (currentAdmin->updateResources(bookId, *globalSystem)) {
            globalSystem->saveData();
            cout << "\nBook marked as available successfully!\n";
        } else {
            cout << "\n[ERROR] Book not found.\n";
        }
    } catch (const exception& e) {
        cout << "\n[ERROR] Error: " << e.what() << "\n";
    }
    pause();
}

void viewAllBooksAdmin() {
    clearScreen();
    cout << "---------- ALL BOOKS IN SYSTEM ----------\n\n";
    
    if (globalSystem->resources.empty()) {
        cout << "No books in system.\n";
        pause();
        return;
    }
    
    cout << left << setw(6) << "ID" << setw(25) << "Title" << setw(18) << "Author" 
         << setw(12) << "Category" << setw(10) << "Status" << setw(8) << "Rating\n";
    cout << string(80, '-') << "\n";
    
    for (auto& r : globalSystem->resources) {
        cout << left << setw(6) << r->getResourceID() 
             << setw(25) << r->getTitle().substr(0, 24)
             << setw(18) << r->getAuthor().substr(0, 17)
             << setw(12) << r->getCategory().substr(0, 11)
             << setw(10) << (r->getAvailability() ? "Available" : "Borrowed")
             << setw(8) << fixed << setprecision(1) << r->getRating() << "\n";
    }
    pause();
}

void viewAllUsers() {
    clearScreen();
    cout << "---------- ALL USERS ----------\n\n";
    
    if (globalSystem->users.empty()) {
        cout << "No users registered.\n";
        pause();
        return;
    }
    
    cout << left << setw(6) << "ID" << setw(25) << "Name" << setw(30) << "Email" 
         << setw(15) << "Membership" << setw(10) << "Balance\n";
    cout << string(90, '-') << "\n";
    
    for (auto& user : globalSystem->users) {
        cout << left << setw(6) << user->getID()
             << setw(25) << user->getFullName().substr(0, 24)
             << setw(30) << user->getEmail().substr(0, 29)
             << setw(15) << user->getMembershipName()
             << setw(10) << "$" << fixed << setprecision(2) << user->getAccountBalance() << "\n";
    }
    pause();
}

void lockUnlockUser() {
    clearScreen();
    cout << "---------- LOCK/UNLOCK USER ----------\n\n";
    
    viewAllUsers();
    
    int userId;
    cout << "Enter User ID to toggle lock/unlock: ";
    cin >> userId;
    cin.ignore();
    
    try {
        for (auto& user : globalSystem->users) {
            if (user && user->getID() == userId) {
                bool wasLocked = user->getLockStatus();
                currentAdmin->lockUser(userId, *globalSystem);
                globalSystem->saveData();
                
                if (wasLocked) {
                    cout << "\nUser account UNLOCKED successfully!\n";
                } else {
                    cout << "\nUser account LOCKED successfully!\n";
                }
                pause();
                return;
            }
        }
        cout << "\n[ERROR] User not found.\n";
    } catch (const exception& e) {
        cout << "\n[ERROR] Error: " << e.what() << "\n";
    }
    pause();
}

void viewBorrowHistoryAdmin() {
    clearScreen();
    cout << "---------- SYSTEM BORROW HISTORY ----------\n\n";
    
    bool hasRecords = false;
    cout << left << setw(15) << "User" << setw(25) << "Book Title" 
         << setw(15) << "Borrow Date" << setw(15) << "Due Date" 
         << setw(12) << "Returned\n";
    cout << string(82, '-') << "\n";
    
    for (const auto user : globalSystem->users) {
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
        cout << "No borrow records.\n";
    }
    pause();
}

void addUserAdmin() {
    clearScreen();
    cout << "---------- ADD NEW USER (ADMIN) ----------\n\n";
    
    string firstName, lastName, email, password;
    double balance = 0;
    
    cout << "First Name: ";
    getline(cin, firstName);
    cout << "Last Name: ";
    getline(cin, lastName);
    cout << "Email: ";
    getline(cin, email);
    cout << "Password (min 6 chars): ";
    getline(cin, password);
    
    bool validBalance = false;
    while (!validBalance) {
        cout << "Initial Balance (Rs.): ";
        if (cin >> balance) {
            if (balance >= 0) {
                validBalance = true;
            } else {
                cout << "[ERROR] Balance must be non-negative.\n";
                cin.clear();
                cin.ignore(10000, '\n');
            }
        } else {
            cout << "[ERROR] Please enter a valid number.\n";
            cin.clear();
            cin.ignore(10000, '\n');
        }
    }
    cin.ignore(10000, '\n');
    
    if (password.length() < 6) {
        cout << "\n[ERROR] Password must be at least 6 characters.\n";
        pause();
        return;
    }
    
    for (auto& user : globalSystem->users) {
        if (user->getEmail() == email) {
            cout << "\n[ERROR] Email already registered.\n";
            pause();
            return;
        }
    }
    
    for (auto& admin : globalSystem->admins) {
        if (admin->getEmail() == email) {
            cout << "\n[ERROR] Email already registered.\n";
            pause();
            return;
        }
    }
    
    try {
        globalSystem->registerUser(firstName, lastName, email, password, balance);
        cout << "\n[SUCCESS] User created successfully!\n";
        cout << "Email: " << email << "\n";
    } catch (...) {
        cout << "\n[ERROR] Error during user creation.\n";
    }
    pause();
}

void addNewAdmin() {
    clearScreen();
    cout << "---------- ADD NEW ADMIN ----------\n\n";
    
    if (currentAdmin->getAccessLevel() != "SuperAdmin") {
        cout << "[ERROR] Only SuperAdmins can create new admins.\n";
        pause();
        return;
    }
    
    string firstName, lastName, email, password, level;
    
    cout << "First Name: ";
    getline(cin, firstName);
    cout << "Last Name: ";
    getline(cin, lastName);
    cout << "Email: ";
    getline(cin, email);
    cout << "Password: ";
    getline(cin, password);
    cout << "Access Level (SuperAdmin/Admin): ";
    getline(cin, level);
    
    if (level != "SuperAdmin" && level != "Admin") {
        cout << "\n[ERROR] Invalid access level.\n";
        pause();
        return;
    }
    
    for (auto& admin : globalSystem->admins) {
        if (admin->getEmail() == email) {
            cout << "\n[ERROR] Email already registered.\n";
            pause();
            return;
        }
    }
    
    try {
        if (currentAdmin->createAdmin(firstName, lastName, email, password, level, *globalSystem)) {
            globalSystem->saveData();
            cout << "\nAdmin created successfully!\n";
        } else {
            cout << "\n[ERROR] Error creating admin.\n";
        }
    } catch (const exception& e) {
        cout << "\n[ERROR] Error: " << e.what() << "\n";
    }
    pause();
}

void viewReports() {
    clearScreen();
    cout << "---------- SYSTEM REPORTS ----------\n\n";
    cout << "1. Customer Report\n";
    cout << "2. Issued Resources Report\n";
    cout << "3. Overdue Resources Report\n";
    cout << "4. Fine Report\n";
    cout << "5. Back to Admin Menu\n";
    cout << "\nSelect report (1-5): ";
    
    int choice;
    cin >> choice;
    cin.ignore();
    
    clearScreen();
    
    try {
        switch (choice) {
            case 1:
                cout << "---------- CUSTOMER REPORT ----------\n\n";
                currentAdmin->generateCustomerReport(*globalSystem);
                break;
            case 2:
                cout << "---------- ISSUED RESOURCES REPORT ----------\n\n";
                currentAdmin->generateIssuedResourcesReport(*globalSystem);
                break;
            case 3:
                cout << "---------- OVERDUE RESOURCES REPORT ----------\n\n";
                currentAdmin->generateOverdueResourcesReport(*globalSystem);
                break;
            case 4:
                cout << "---------- FINE REPORT ----------\n\n";
                currentAdmin->generateFineReport(*globalSystem);
                break;
            case 5:
                return;
            default:
                cout << "[ERROR] Invalid option.\n";
        }
    } catch (const exception& e) {
        cout << "[ERROR] Error generating report: " << e.what() << "\n";
    }
    pause();
}

void updateUserMembership() {
    clearScreen();
    cout << "---------- UPDATE USER MEMBERSHIP ----------\n\n";
    
    if (globalSystem->users.empty()) {
        cout << "No users in the system.\n";
        pause();
        return;
    }
    
    // Display all users
    cout << "Available Users:\n\n";
    cout << left << setw(8) << "User ID" << setw(25) << "Name" << setw(30) << "Email" 
         << setw(15) << "Membership\n";
    cout << string(80, '-') << "\n";
    
    for (auto& user : globalSystem->users) {
        if (user) {
            cout << left << setw(8) << user->getID() 
                 << setw(25) << user->getFullName().substr(0, 24)
                 << setw(30) << user->getEmail().substr(0, 29)
                 << setw(15) << user->getMembershipName() << "\n";
        }
    }
    
    cout << "\n";
    int userId, tier;
    cout << "Enter User ID to update: ";
    cin >> userId;
    cin.ignore();
    
    // Verify user exists
    User* selectedUser = nullptr;
    for (auto& user : globalSystem->users) {
        if (user && user->getID() == userId) {
            selectedUser = user;
            break;
        }
    }
    
    if (!selectedUser) {
        cout << "\n[ERROR] User not found.\n";
        pause();
        return;
    }
    
    cout << "\nCurrent Membership: " << selectedUser->getMembershipName() << "\n\n";
    cout << "Membership Tiers:\n";
    cout << "  1 = Normal   (2 books, 0% discount, Essential)\n";
    cout << "  2 = Extra    (5 books, 25% discount, Premium)\n";
    cout << "  3 = Deluxe   (10 books, 50% discount, VIP)\n\n";
    
    cout << "Select new membership tier (1-3): ";
    cin >> tier;
    cin.ignore();
    
    if (tier < 1 || tier > 3) {
        cout << "\n[ERROR] Invalid tier. Must be 1, 2, or 3.\n";
        pause();
        return;
    }
    
    try {
        currentAdmin->assignMembershipTier(userId, tier, *globalSystem);
        globalSystem->saveData();
        cout << "\n Membership updated successfully!\n";
        cout << "User: " << selectedUser->getFullName() << "\n";
        cout << "New Membership: " << selectedUser->getMembershipName() << "\n";
    } catch (const exception& e) {
        cout << "\n[ERROR] Error: " << e.what() << "\n";
    }
    pause();
}

void adminLogout() {
    currentAdmin = nullptr;
    cout << "\nLogged out successfully.\n";
    pause();
}


// ==================== SESSION LOOPS ====================

void userSession() {
    while (currentUser != nullptr) {
        clearScreen();
        displayUserMenu();
        
        int choice;
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "\nInvalid input. Please enter a number.\n";
            pause();
            continue;
        }
        cin.ignore(10000, '\n');
        
        switch (choice) {
            case 1: viewBooks(); break;
            case 2: viewBookDetails(); break;
            case 3: viewBooksByFavoriteCategories(); break;
            case 4: borrowBook(); break;
            case 5: returnBook(); break;
            case 6: viewBorrowingHistory(); break;
            case 7: viewProfile(); break;
            case 8: updateProfile(); break;
            case 9: viewMembershipDetails(); break;
            case 10: changeMembershipTier(); break;
            case 11: viewLoyaltySummary(); break;
            case 12: redeemLoyaltyPoints(); break;
            case 13: redeemFineFreePass(); break;
            case 14: upgradeMembershipWithPoints(); break;
            case 15: rechargeBalance(); break;
            case 16: viewBookReviews(); break;
            case 17: userLogout(); break;
            default: cout << "\n[ERROR] Invalid option.\n"; pause();
        }
    }
}

void adminSession() {
    while (currentAdmin != nullptr) {
        clearScreen();
        displayAdminMenu();
        
        int choice;
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "\n[ERROR] Invalid input. Please enter a number.\n";
            pause();
            continue;
        }
        cin.ignore(10000, '\n');
        
        switch (choice) {
            case 1: addBook(); break;
            case 2: removeBook(); break;
            case 3: updateBook(); break;
            case 4: viewAllBooksAdmin(); break;
            case 5: viewAllUsers(); break;
            case 6: addUserAdmin(); break;
            case 7: lockUnlockUser(); break;
            case 8: viewBorrowHistoryAdmin(); break;
            case 9: addNewAdmin(); break;
            case 10: viewReports(); break;
            case 11: updateUserMembership(); break;
            case 12: adminLogout(); break;
            default: cout << "\n[ERROR] Invalid option.\n"; pause();
        }
    }
}

// ==================== MAIN ====================

int main() {
    globalSystem = new LibrarySystem();
    
    cout << "Library Management System - Loading...\n";
    
    bool running = true;
    while (running) {
        displayMainMenu();
        
        int choice;
        cin >> choice;
        cin.ignore();
        
        switch (choice) {
            case 1:
                userLogin();
                if (currentUser != nullptr) userSession();
                break;
            case 2:
                adminLogin();
                if (currentAdmin != nullptr) adminSession();
                break;
            case 3:
                registerUser();
                break;
            case 4:
                running = false;
                cout << "\nThank you for using Library Management System!\n";
                break;
            default:
                cout << "\n[ERROR] Invalid option. Please try again.\n";
                pause();
        }
    }
    
    delete globalSystem;
    return 0;
}

