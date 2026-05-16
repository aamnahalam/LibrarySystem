================================================================================
                      LIBRARY MANAGEMENT SYSTEM
                         OOP Project - Final 2
================================================================================

================================================================================
STUDENT INFORMATION
================================================================================

Project Team Members:

1. Full Name:              Depesh Kumar Jewani
   Roll Number:           CS-25091
   
2. Full Name:              Abdul Rafay Irfan
   Roll Number:           CS-25092
   
3. Full Name:              Hiba Zia
   Roll Number:           CS-25102
   
4. Full Name:              Syeda Aamnah Alam
   Roll Number:           CS-25105

Section:                   B & C


================================================================================
PROJECT OVERVIEW
================================================================================

This is a comprehensive Library Management System built in C++ demonstrating 
advanced Object-Oriented Programming concepts through a Command-Line Interface (CLI).

The system manages users, admins, resources (books), memberships, and borrowing 
transactions with advanced features like loyalty points, fine calculations, 
and membership tiers.


================================================================================
STEPS TO RUN THE PROGRAM
================================================================================

PREREQUISITES:
- Windows Operating System
- C++ compiler (g++ - MinGW or GCC)
- Command Prompt or PowerShell terminal


COMPILATION & EXECUTION
========================

1. Open Command Prompt or PowerShell

2. Navigate to the LibrarySystem directory:
   cd "c:\Users\my pc\OneDrive\Desktop\oop  project final 2\LibrarySystem"

3. Compile using g++ command:

   For Command Prompt:
   g++ main_cli.cpp src/core/*.cpp src/users/*.cpp src/resources/*.cpp ^^
       src/Membership/*.cpp src/exceptions/*.cpp src/transactions/*.cpp ^^
       src/services/*.cpp -o library.exe -std=c++17

   For PowerShell (single line):
   g++ main_cli.cpp src/core/*.cpp src/users/*.cpp src/resources/*.cpp src/Membership/*.cpp src/exceptions/*.cpp src/transactions/*.cpp src/services/*.cpp -o library.exe -std=c++17

   This compiles main_cli.cpp and all source files into library.exe

4. Run the compiled executable:

   For Command Prompt:
   library.exe

   For PowerShell:
   .\library.exe

5. The program starts with the main menu displaying 4 options:
   - User Login
   - Admin Login
   - Register New User
   - Exit

6. Follow on-screen prompts to navigate the system


INITIAL TEST DATA
=================

The system comes with pre-loaded test data in text files:
- users.txt: Contains sample users for testing
- admins.txt: Contains sample admin accounts
- resources.txt: Contains sample books in catalog
- borrow_history.txt: Contains sample borrow transactions
- reviews.txt: Contains book reviews

Test Admin Credentials:
- Admin Login: Email: admin@library.com | Password: admin123
- Manager Login: Email: manager@library.com | Password: manager123

Test User Credentials:
- Email: ali@email.com | Password: 1234
- Register a new user: Select option 3 from main menu


QUICK START EXAMPLE - USER FLOW
=================================

1. Start Program:
   library.exe

2. Main Menu Appears:
   ============================================================
   LIBRARY MANAGEMENT SYSTEM v2.0 - CLI VERSION
   ============================================================
   
   1. User Login
   2. Admin Login
   3. Register New User
   4. Exit
   
   Select option (1-4): 

3. Option 1: User Login
   - Enter email: ali@email.com
   - Enter password: 1234
   - If successful, logged in user menu appears

4. User Menu (21 Options Available):
   1. View Available Books - See all books in catalog
   2. Search & Filter Books - Search by title, author, category or filter by status
   3. View Book Details - Detailed info on specific book
   4. View Books in My Favorite Categories - Personalized list
   5. Borrow a Book - Borrow from available books
   6. Return a Book - Return borrowed book
   7. Reserve a Book - Reserve a currently borrowed book
   8. View My Reservations - View, cancel, or collect reservations
   9. View Reservation Queue - Check your position in a book's queue
   10. View My Borrowing History - See all past transactions
   11. View My Profile - See personal info & account status
   12. Update Profile - Modify name/email/password
   13. View Membership Details - Check tier benefits
   14. Change Membership Tier - Upgrade/downgrade
   15. View Loyalty Points Summary - Check point balance
   16. Redeem Loyalty Points for Discount - Convert points to balance
   17. Redeem Points for Fine Waiver - Get next fine waived
   18. Upgrade Membership with Loyalty Points - Premium upgrade
   19. Recharge Account Balance - Add funds
   20. View Book Reviews - See ratings & reviews
   21. Logout - End session

5. Example: Borrow a Book (Option 4)
   - View list of available books with ID, title, author, rating
   - Enter book ID to borrow
   - System confirms borrow with due date (14 days)
   - Fine rate shown based on book type
   - Loyalty points earned (+5 points)

6. Example: Return a Book (Option 5)
   - View list of currently borrowed books
   - Enter book ID to return
   - System calculates fine if overdue
   - Option to submit review after return
   - Loyalty points earned for on-time return (+10 points)


QUICK START EXAMPLE - ADMIN FLOW
=================================

1. Start Program:
   library.exe

2. Select Option 2: Admin Login
   - Enter email: admin@library.com
   - Enter password: admin123

3. Admin Menu (13 Options Available):
   1. Add New Book - Add book to catalog
   2. Remove Book - Delete book from system
   3. Update Book - Modify book details
   4. View All Books - Browse complete catalog
   5. View All Users - See all registered users
   6. Add New User - Create user account
   7. Lock/Unlock User - Suspend/restore user
   8. View Borrow History - See all transactions
   9. Add New Admin - Create admin account (SuperAdmin only)
   10. View Reports - Statistics and analytics
   11. Update User Membership - Change membership tier
   12. Manage Reservations - View, cancel, fulfill reservations and check queues
   13. Logout - End session

4. Example: Add New Book (Option 1)
   - Enter title, author, category
   - Select book type: 1=PrimePick, 2=ClassicShelf, 3=BudgetPick
   - Book automatically gets new ID and added to catalog

5. Example: View All Books (Option 4)
   - Displays all books with ID, title, author, category, status, rating
   - Shows availability (Available/Borrowed)
   - Shows fine rate per day


SCREEN NAVIGATION TIPS
======================

- Press Enter to continue after each operation
- Main menu always asks "Select option (1-X): "
- Enter 0 to cancel/go back in selection screens
- System validates input and shows [ERROR] messages if invalid
- All changes saved automatically to text files
- Current user info shown at top of user menu
- Current balance and loyalty points displayed


FILE STRUCTURE
==============

LibrarySystem/
├── main_cli.cpp                 [Main CLI application - PRIMARY ENTRY POINT]
├── README.txt                   [This file]
├── users.txt                    [User data persistence file]
├── admins.txt                   [Admin data persistence file]
├── resources.txt                [Book catalog data file]
├── borrow_history.txt           [Transaction history file]
├── reviews.txt                  [Book reviews data file]
├── src/
│   ├── core/
│   │   └── LibrarySystem.h/cpp   [Core system manager]
│   ├── users/
│   │   ├── Person.h/cpp          [Base person class]
│   │   ├── user.h/cpp            [User class - implements 21 CLI features]
│   │   └── admin.h/cpp           [Admin class - implements 13 CLI features]
│   ├── Membership/
│   │   ├── Membership.h/cpp      [Base membership class]
│   │   ├── NormalMembership.h/cpp [Normal tier - max 2 books, no fine discount]
│   │   ├── ExtraMembership.h/cpp  [Extra tier - max 5 books, 25% fine discount]
│   │   └── DeluxeMembership.h/cpp [Deluxe tier (HIGHEST) - max 10 books, 50% fine discount]
│   ├── resources/
│   │   ├── Resource.h/cpp        [Base resource class]
│   │   ├── PrimePickBook.h/cpp    [Premium book - $5.0/day fine rate]
│   │   ├── ClassicShelfBook.h/cpp [Standard book - $3.0/day fine rate]
│   │   └── BudgetPickBook.h/cpp   [Budget book - $2.0/day fine rate]
│   ├── transactions/
│   │   ├── BorrowRecord.h/cpp     [Borrow transaction record]
│   │   └── Reservation.h/cpp      [Reservation queue management]
│   ├── services/
│   │   └── Review.h/cpp           [Book review service]
│   └── exceptions/
│       ├── LibraryException.h/cpp [Base exception class]
│       ├── BorrowLimitExceededException.h/cpp
│       ├── InsufficientBalanceException.h/cpp
│       └── ResourceNotAvailableException.h/cpp


================================================================================
OOP CONCEPTS DEMONSTRATED
================================================================================

1. INHERITANCE
===============
   Description: Base class extended by derived classes to inherit properties 
                and methods.

   Implementations:
   - Resource (Base Class) → PrimePickBook, ClassicShelfBook, BudgetPickBook
     Classes: src/resources/Resource.h (base)
     Derived: src/resources/PrimePickBook.h, ClassicShelfBook.h, BudgetPickBook.h
     Demonstrates: Each book type inherits common properties (title, author, 
                   rating) and behaviors (borrowing, rating)

   - Membership (Base Class) → NormalMembership, DeluxeMembership, ExtraMembership
     Classes: src/Membership/Membership.h (base)
     Derived: src/Membership/*.h
     Demonstrates: Different membership tiers inherit base membership functionality

   - Person (Base Class) → User, Admin
     Classes: src/users/Person.h (base)
     Derived: src/users/user.h, admin.h
     Demonstrates: Both users and admins share common person attributes

   - LibraryException (Base Class) → Multiple specific exceptions
     Classes: src/exceptions/LibraryException.h (base)
     Derived: BorrowLimitExceededException, InsufficientBalanceException, etc.


2. POLYMORPHISM (Method Overriding)
====================================
   Description: Derived classes override base class virtual methods to provide 
                specialized implementations.

   Implementations:
   - Virtual Function: getFineRate()
     Base Class: Resource::getFineRate()
     Overridden In: PrimePickBook (returns 5.0), ClassicShelfBook (returns 3.0), 
                    BudgetPickBook (returns 2.0)
     Usage: user.cpp, line 203 - calculates fines based on book type
     Code: double fine = lateDays * r->getFineRate();

   - Virtual Function: displayDetails()
     Base Class: Resource::displayDetails() (pure virtual)
     Overridden In: PrimePickBook, ClassicShelfBook, BudgetPickBook
     Purpose: Each book type can display different detailed information

   - Virtual Function: getMaxBorrowLimit()
     Base Class: Membership::getMaxBorrowLimit()
     Overridden In: NormalMembership (return 3), DeluxeMembership (return 5), 
                    ExtraMembership (return 10)
     Usage: Membership tier determines how many books a user can borrow


3. ENCAPSULATION (Data Hiding)
===============================
   Description: Private/protected members and public methods control access to data.

   Implementations:
   - User Class: src/users/user.h
     Private: firstName, lastName, email, password, balance, membershipTier, 
             loyaltyPoints, borrowedBooks
     Public Methods: viewProfile(), updateProfile(), borrowBook(), returnBook(), 
                    redeemPoints()
     Demonstrates: User data is protected from direct external modification; 
                   access controlled through public methods

   - Resource Class: src/resources/Resource.h
     Protected: resourceID, title, author, category, isAvailable, rating, 
               borrowCount, reviews
     Public Methods: getTitle(), getAuthor(), getAvailability(), addRating(), 
                    updateAvailability()
     Demonstrates: Book data cannot be changed arbitrarily; only specific 
                   operations allowed

   - Admin Class: src/users/admin.h
     Private: adminID, accessLevel
     Public Methods: addBook(), removeBook(), manageMemberships(), 
                    generateReports()
     Demonstrates: Admin privileges protected; only admin operations available


4. ABSTRACTION (Abstract Classes and Interfaces)
================================================
   Description: Define interfaces through abstract classes; hide implementation 
                details.

   Implementations:
   - Resource (Abstract Base Class): src/resources/Resource.h
     Pure Virtual Methods: displayDetails(), getFineRate()
     Usage: Each derived class (PrimePickBook, ClassicShelfBook, BudgetPickBook) 
            must implement these methods
     File: src/resources/Resource.h, lines 40-41
     Demonstrates: System only knows about Resource interface; details hidden 
                   in derived classes

   - Membership (Abstract Base Class): src/Membership/Membership.h
     Pure Virtual Methods: getMaxBorrowLimit(), getFineDiscount(), 
                          getMonthlyPrice(), getPerks(), displayDetails()
     Usage: Each membership tier implements these to define its characteristics
     File: src/Membership/Membership.h
     Demonstrates: Membership interface defined; implementation varies by tier

   - Exception Handling: src/exceptions/LibraryException.h (base exception)
     Abstract pattern used for custom exceptions
     Demonstrates: All custom exceptions inherit from LibraryException base


5. COMPOSITION (Has-A Relationship)
====================================
   Description: Objects contain other objects; aggregate functionality.

   Implementations:
   - LibrarySystem Class: src/core/LibrarySystem.h
     Contains: vector<User*> users, vector<Admin*> admins, 
              vector<Resource*> resources, vector<BorrowRecord*> borrowRecords
     Code: lines 17-20 in LibrarySystem.h
     Demonstrates: Library system manages collections of users, admins, and 
                   resources

   - User Class: src/users/user.h
     Contains: Membership object, vector of borrowed books, BorrowRecord objects
     Demonstrates: User "has-a" membership and "has-a" collection of 
                   borrowed resources

   - Resource Class: src/resources/Resource.h
     Contains: vector<Review*> reviews
     Code: line 20 in Resource.h
     Demonstrates: Each resource "has-a" collection of review objects

   - BorrowRecord Class: src/transactions/BorrowRecord.h
     Contains: User reference, Resource reference, dates, fine amount
     Demonstrates: Transaction records compose user and resource information


6. EXCEPTION HANDLING (Custom Exceptions)
==========================================
   Description: Custom exception classes for specific error conditions.

   Implementations:
   - BorrowLimitExceededException: src/exceptions/BorrowLimitExceededException.h
     Usage: Thrown when user tries to borrow more books than membership allows
     Caught In: borrowBook(), searchAndFilterBooks() in main_cli.cpp
     Example: If user has Normal membership (max 2 books) and tries to
             borrow a 3rd book

   - InsufficientBalanceException: src/exceptions/InsufficientBalanceException.h
     Usage: Thrown when user tries to pay fine or upgrade without sufficient balance
     Caught In: registerUser(), borrowBook(), changeMembershipTier() in main_cli.cpp
     Example: User owes $5 fine but only has $2 balance

   - ResourceNotAvailableException: src/exceptions/ResourceNotAvailableException.h
     Usage: Thrown when user tries to borrow an unavailable resource
     Caught In: borrowBook(), searchAndFilterBooks() in main_cli.cpp
     Example: Book is already borrowed by another user

   Base Exception Class: src/exceptions/LibraryException.h
     All custom exceptions inherit from this base class
     Provides: Unified error handling and what() method
     30+ try-catch blocks throughout main_cli.cpp covering all transactional
     operations (login, borrow, return, reserve, membership change, etc.)


7. AGGREGATION (Weak Composition)
==================================
   Description: Objects refer to other objects; objects can exist independently.

   Implementations:
   - User borrows Book: User object references Resource objects via pointers
     Does NOT own resources (resources are managed by LibrarySystem)
     Code: src/users/user.cpp - borrowBook() method
     Demonstrates: User "uses" resources but system retains ownership

   - Admin manages Books: Admin performs operations on Resource objects
     Admin does NOT own resources
     Code: src/users/admin.cpp - addBook(), removeBook() methods
     Demonstrates: Admin works with resources without owning them


8. DATA PERSISTENCE (File I/O)
===============================
   Description: Save and load object data from text files.

   Implementations:
   - Users Persistence: src/core/LibrarySystem.cpp - saveData(), loadData()
     File: users.txt
     Format: ID | Name | Email | Password | Balance | Membership | LoyaltyPoints | Locked
     Demonstrates: User objects serialized to/from text file

   - Resources Persistence:
     File: resources.txt
     Format: ID | Type | Title | Author | Category | Available | Rating | BorrowCount
     Demonstrates: Resource objects serialized to/from text file

   - Transaction History:
     File: borrow_history.txt
     Demonstrates: BorrowRecord objects persisted for audit trail

   - Admin Data:
     File: admins.txt
     Demonstrates: Admin objects persisted with access levels


9. ASSOCIATION (Object Relationships)
======================================
   Description: Objects know about and interact with each other.

   Implementations:
   - User-Library Relationship:
     User is registered in LibrarySystem's user vector
     Code: src/core/LibrarySystem.h, line 17
     
   - Borrow Relationship:
     User associated with Resource through BorrowRecord
     Code: src/transactions/BorrowRecord.h
     Demonstrates: Many-to-many relationship (user can borrow many resources; 
                   resource can be borrowed by many users over time)

   - Review-Resource Relationship:
     Review object associated with Resource
     Code: src/resources/Resource.h, line 20 - vector<Review*> reviews
     Demonstrates: One-to-many relationship


10. METHOD OVERLOADING (Parametric Polymorphism)
==================================================
    Description: Multiple methods with same name, different parameters.

    Implementations:
    - Constructor Overloading:
      LibrarySystem has multiple constructors for different initialization paths
      Code: src/core/LibrarySystem.h

    - Function Overloading in User:
      Demonstrated through different update profile methods
      Code: src/users/user.cpp - updateProfile() with optional parameters


11. FRIEND CLASSES
===================
    Description: Classes granted access to private members of other classes.

    Implementations:
    - LibrarySystem declares Admin as friend
      Code: src/core/LibrarySystem.h, line 55
      Purpose: Admin needs access to private LibrarySystem data to manage
              users and resources
      Demonstrates: Controlled access to private members through friendship


12. INPUT VALIDATION (Defensive Programming)  [NEW]
=====================================================
    Description: Dedicated helper functions ensure all user input is valid
                 before processing, preventing crashes from bad input.

    Implementations:
    - getValidIntInput(minVal, maxVal, prompt): main_cli.cpp, line 69
      Validates integer is within an expected range; loops until valid
      Used for: all menu selections throughout the program

    - getValidDoubleInput(minVal, prompt): main_cli.cpp, line 92
      Validates decimal/float is above a minimum value (e.g., > 0)
      Used for: balance recharge, membership cost validation

    - getValidEmail(prompt): main_cli.cpp, line 163
      Validates email contains '@', valid domain, and legal characters
      Used for: user registration, profile updates

    - getValidDate(prompt): main_cli.cpp, line 183
      Validates YYYY-MM-DD format with range checks on year, month, day
      Used for: return date entry (testing mode)

    - getValidYesNoInput(prompt): main_cli.cpp, line 203
      Accepts only 'y'/'Y' or 'n'/'N'; rejects anything else
      Used for: confirmation dialogs throughout the CLI

    - getValidBookID(prompt): main_cli.cpp, line 220
      Accepts 0 (go back) or IDs in the range 5000–9999
      Used for: book selection screens


================================================================================
FEATURE SUMMARY - CLI IMPLEMENTATION
================================================================================

USER FEATURES (21 Options in CLI Menu)
=========================================

1. View Available Books
   - Lists all books in catalog with formatted table
   - Shows: ID, Title, Author, Category, Status, Rating
   - Status: Available or Borrowed
   - Displays star ratings (1-5)

2. Search & Filter Books  [NEW]
   - Submenu with 5 options:
     1. Search by Title    - Partial/full title match
     2. Search by Author   - Partial/full author name match
     3. Search by Category - Filter by category (Fiction, Fantasy, History, etc.)
     4. View Available Books - Show only books currently available to borrow
     5. View Borrowed Books  - Show only books currently checked out
   - Results displayed in formatted table (ID, Title, Author, Category, Status, Rating)
   - Option to borrow a book directly from search results
   - Enter 0 to go back to main menu

3. View Book Details
   - Select specific book by ID
   - Shows complete book information
   - Displays: Title, Author, Category, Rating, Borrow Count
   - Shows fine rate per day (varies by book type)
   - Indicates if book is new arrival

4. View Books in My Favorite Categories
   - Analyzes user's borrow history
   - Identifies favorite categories
   - Filters books by those categories
   - Sorts by rating (highest first)
   - Shows total matching books

5. Borrow a Book
   - Shows only available books
   - User enters book ID to borrow
   - Validates: Account balance, borrow limit, book availability
   - Shows due date (14 days from borrow)
   - Blocks account if locked or has unpaid fines
   - Earns 5 loyalty points per borrow

6. Return a Book
   - Shows only currently borrowed books
   - User enters book ID to return
   - Calculates fine if overdue
   - Fine amount deducted from account balance
   - Offers to submit review after return
   - Earns 10 loyalty points for on-time returns
   - Option to view book reviews after return
   - Automatically fulfills next pending reservation for returned book

7. Reserve a Book  [NEW]
   - Allows reserving any book (available or currently borrowed)
   - Creates reservation with today's date and queue position
   - Shows confirmation with reservation date
   - Each book has a FIFO reservation queue

8. View My Reservations  [NEW]
   - Lists all pending and fulfilled reservations
   - Shows queue position for pending reservations
   - Shows [READY] tag for fulfilled reservations available to collect
   - Submenu actions for each reservation:
     1. Cancel a pending reservation
     2. Collect a ready (fulfilled) book
     3. Skip
   - Cancelled/collected reservations removed from active list

9. View Reservation Queue  [NEW]
   - Enter a book ID to see your position in that book's queue
   - Shows how many people are ahead of you
   - Enter 0 to view all your reservations with queue positions

10. View My Borrowing History
    - Shows all past borrow/return transactions
    - Displays: Resource name, borrow date, return date, fines
    - Complete audit trail of user activity

11. View My Profile
    - Shows: Name, Email, ID, Account Balance
    - Shows: Membership Tier, Loyalty Points
    - Quick reference for account status

12. Update Profile
    - Modify first name
    - Modify last name
    - Modify email address
    - Modify password
    - All fields optional (leave blank to keep current)
    - Changes saved immediately

13. View Membership Details
    - Shows current membership tier
    - Displays max borrow limit for tier
    - Shows fine discount percentage
    - Lists membership perks and benefits

14. Change Membership Tier
    - Shows all 3 membership options: Normal, Deluxe, Extra
    - Confirms tier benefits and costs
    - Validates sufficient balance for upgrades
    - Shows cost differences
    - User must confirm major changes

15. View Loyalty Points Summary
    - Shows current loyalty point balance
    - Displays points earned from borrowing/returns
    - Shows points used for redemptions
    - Lists available redemption options

16. Redeem Loyalty Points for Discount
    - Converts loyalty points to account credit
    - Conversion: 100 points = Rs.50 discount
    - Must be multiple of 100
    - Points immediately added to account balance
    - Updates displayed balance

17. Redeem Points for Fine Waiver
    - Cost: 100 loyalty points
    - Benefit: Next overdue fine waived
    - Validates sufficient points
    - Activates fine waiver flag

18. Upgrade Membership with Loyalty Points
    - Cost: 1000 loyalty points
    - Upgrade to Deluxe membership (highest tier)
    - Benefits: Borrow up to 10 books, 50% fine discount, free passes
    - Confirms before deducting points
    - Validates sufficient points balance

19. Recharge Account Balance
    - Shows current balance
    - User enters amount to add
    - Validates amount > 0
    - Updates balance immediately
    - Used for paying fines or maintaining balance

20. View Book Reviews
    - Browse all books with average ratings
    - View detailed reviews from other users
    - See review text and ratings
    - Submit new reviews for books

21. Logout
    - Clears current user session
    - Returns to main menu


ADMIN FEATURES (13 Options in CLI Menu)
==========================================

1. Add New Book
   - Admin enters: title, author, category
   - Selects book type: 1=PrimePick, 2=ClassicShelf, 3=BudgetPick
   - System auto-assigns ID
   - Book added to catalog immediately
   - Saved to resources.txt

2. Remove Book
   - Admin enters book ID to remove
   - Confirms deletion
   - Book removed from catalog

3. Update Book
   - Admin selects book to modify
   - Can update: title, author, category
   - Changes saved immediately

4. View All Books
   - Lists complete catalog
   - Shows: ID, Title, Author, Category, Status, Rating
   - Shows availability and fine rates
   - Shows borrow count and review score

5. View All Users
   - Lists all registered users
   - Shows: ID, Name, Email, Balance, Membership, Status
   - Shows lock status (locked/active)
   - Shows loyalty points

6. Add New User
   - Admin manually creates user account
   - Enters: name, email, password, initial balance
   - User automatically added to system
   - Can set initial membership tier

7. Lock/Unlock User
   - Admin selects user by ID
   - Can lock: prevents borrowing
   - Can unlock: restores borrowing privileges
   - Useful for overdue/unpaid accounts

8. View Borrow History
   - Shows all transactions in system
   - Format: User, Resource, Borrow Date, Return Date, Fine
   - Complete transaction audit trail

9. Add New Admin
   - Only SuperAdmin can create new admins
   - Enter: name, email, password, access level
   - Assigns access level: SuperAdmin or Admin
   - Admin account added to system

10. View Reports
    - Statistics: Total users, total books, total transactions
    - Revenue statistics: Fine revenue, total balance
    - Usage statistics: Most borrowed books, active users
    - Summary dashboard

11. Update User Membership
    - Select user by ID
    - Choose new membership tier: Normal, Deluxe, Extra
    - Membership immediately updated
    - Benefits/limits change instantly

12. Manage Reservations  [NEW]
    - View all active reservations in the system
    - Cancel a specific reservation by user and book ID
    - Fulfill next pending reservation for a book (mark as ready)
    - Check reservation queue for any book
    - Helps admin process returns and notify waiting users

13. Logout
    - Clears current admin session
    - Returns to main menu


CORE FUNCTIONALITY (CLI-Based)
==================================

User Registration & Authentication
  - Email/password based login
  - Password minimum 6 characters
  - Email uniqueness validation
  - Separate user and admin authentication

Book Catalog Management
  - Add books with type classification
  - Remove books from catalog
  - Update book information
  - Dynamic ID assignment

Membership System
  - Three tiers: Normal (max 2), Extra (max 5), Deluxe (max 10 - HIGHEST)
  - Membership affects: borrow limit, fine discount, price
  - Users can change tiers (upgrade/downgrade)
  - Tier benefits displayed clearly

Fine Calculation & Payment
  - Different rates per book type:
    * PrimePickBook: Rs.5.0/day
    * ClassicShelfBook: Rs.3.0/day
    * BudgetPickBook: Rs.2.0/day
  - Calculated on return if overdue
  - Deducted from account balance
  - Fine waiver option available

Loyalty Points System
  - Earn 5 points per borrow
  - Earn 10 points per on-time return
  - Lose points when redeemed
  - Redemption options:
    * 100 points = Rs.50 discount
    * 100 points = next fine waived
    * 1000 points = membership upgrade

Review & Rating System
  - Users can rate books 1-5 stars
  - Users can submit text reviews
  - Average rating calculated
  - Reviews displayed per book
  - Review count tracked

File Persistence
  - All data automatically saved to text files
  - users.txt: user accounts and profiles
  - admins.txt: admin accounts
  - resources.txt: book catalog
  - borrow_history.txt: all transactions
  - reviews.txt: book reviews

Account Management
  - Account balance tracking
  - Balance recharge functionality
  - Locked/active account status
  - Account lockout for violations

Book Availability Tracking
  - Real-time availability status
  - Updates on borrow/return
  - Prevents overborrowing
  - Category tracking

Search & Filter System  [NEW]
  - Search books by title (partial match supported)
  - Search books by author (partial match supported)
  - Search books by category
  - Filter to show only available books
  - Filter to show only currently borrowed books
  - Results shown in formatted table
  - Option to borrow directly from search results

Reservation System  [NEW]
  - FIFO queue per book: first reserved = first fulfilled
  - Users can reserve any book (available or borrowed)
  - Queue position tracked and displayed
  - Status lifecycle: pending → fulfilled → collected / cancelled
  - Auto-fulfillment: when a book is returned, next pending reservation
    is automatically marked fulfilled and ready for collection
  - Admins can view all queues, cancel or manually fulfill reservations
  - Data persisted in reservations.txt

Input Validation  [NEW]
  - All numeric inputs validated for type and range; [INPUT ERROR] shown on failure
  - Email format validated (must contain @ and valid domain)
  - Date format validated (YYYY-MM-DD, year 2000-2100, valid month/day)
  - Yes/No inputs validated (only y/Y or n/N accepted)
  - Book IDs validated (0 = go back, or 5000–9999)
  - Passwords validated (minimum 6 characters)
  - All inputs loop until valid — no crashes on bad input


================================================================================
QUICK TEST CASES
================================================================================

1. User Login: admin@library.com / admin123 (test credentials provided above)
2. Borrow a book and return it to test fine calculation
3. Check data persistence by closing and reopening the program
4. Try borrowing beyond membership limit to test error handling
5. Search for a book by title using option 2 in the user menu  [NEW]
6. Reserve a borrowed book and check your queue position (options 7-9)  [NEW]
7. Enter invalid input (letters in number field) to test input validation  [NEW]


================================================================================
FUTURE IMPROVEMENTS & RECOMMENDATIONS
================================================================================

Potential enhancements for future versions:

1. Migrate to database (SQLite/MySQL) - 100x faster searches
2. Add search functionality - Search books by title, author, category  [IMPLEMENTED]
3. Implement password hashing - Security critical for production
4. Add colored console output - Better user experience
5. Implement pagination - Reduce console clutter
6. Book recommendation system - Based on borrow history
7. Email notifications - Registration, overdue, availability alerts
8. Advanced reporting - Revenue trends, usage analytics
9. Role-based access control - Fine-grained permissions
10. Data backup & recovery - Automated daily backups


================================================================================
CONCLUSION
================================================================================

This is our Library Management System that we made for the OOP project. 
It's basically a CLI application where users and admins can login and manage 
a library system.

What we did:
- Created a system with users, admins, books, and transactions
- Implemented different membership tiers (Normal, Deluxe, Extra)
- Added fine calculation for overdue books
- Made a loyalty points system
- Stored everything in text files so data stays even after closing
- Added search & filter system (by title, author, category, availability)
- Built a reservation queue system with FIFO ordering and auto-fulfillment
- Added full input validation across all menus to prevent crashes on bad input
- Implemented custom exception handling (30+ try-catch blocks)

OOP concepts we used:
- Inheritance: Different book types and membership tiers inherit from base classes
- Polymorphism: Virtual functions for fine rates that change by book type
- Encapsulation: Private data members with public methods to access them
- Abstraction: Abstract base classes define interfaces
- Composition: Library system contains users and resources
- Exception handling: Custom exceptions for different error cases

Features that work:
- User registration and login
- Borrowing and returning books
- Paying fines
- Loyalty points
- Changing membership tiers
- Admin controls for managing books and users
- Search and filter books by title, author, category, or availability
- Reservation queue system with position tracking and auto-fulfillment
- Input validation on all menus (no crashes from bad input)
- Custom exception handling for borrow limit, balance, and availability errors

What's good about it:
- All the basic library stuff works
- Data saves between sessions
- Both users and admins have their own menus
- Fine calculation is correct based on book type
- It's pretty easy to use

What could be better:
- We used text files instead of a database (works but slow)
- Passwords are stored as plain text (not secure)
- It's only tested with small amounts of data
- Some dead code we didn't clean up

This project helped us learn about OOP concepts like inheritance and polymorphism.
We could improve it later by using a database instead of text files or adding 
more features like email notifications.

Thanks for checking out our project!


================================================================================
END OF README
================================================================================
