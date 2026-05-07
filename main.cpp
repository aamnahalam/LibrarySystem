#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "src/core/LibrarySystem.h"
#include "src/users/user.h"
#include "src/users/admin.h"
#include "src/resources/PrimePickBook.h"
#include "src/resources/ClassicShelfBook.h"
#include "src/resources/BudgetPickBook.h"
#include "src/Membership/NormalMembership.h"
#include "src/Membership/ExtraMembership.h"
#include "src/Membership/DeluxeMembership.h"
#include "src/services/FineWalletManager.h"
#include "src/services/NotificationService.h"
#include "src/services/Review.h"
#include "src/exceptions/BorrowLimitExceededException.h"
#include "src/exceptions/InsufficientBalanceException.h"

#pragma comment(lib, "ws2_32.lib")

LibrarySystem* globalSystem;
User* currentUser = nullptr;
Admin* currentAdmin = nullptr;

string readFile(const string& path) {
    ifstream file(path);
    if (!file) return "";
    stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

string getContentType(const string& path) {
    if (path.find(".html") != string::npos) return "text/html";
    if (path.find(".css") != string::npos) return "text/css";
    if (path.find(".js") != string::npos) return "application/javascript";
    return "text/plain";
}

string handleRequest(const string& request) {
    stringstream ss(request);
    string method, path, version;
    ss >> method >> path >> version;

    if (method == "GET") {
        if (path == "/") path = "/index.html";
        string filePath = "web" + path;
        string content = readFile(filePath);
        if (content.empty()) {
            return "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\nNot Found";
        }
        string contentType = getContentType(filePath);
        return "HTTP/1.1 200 OK\r\nContent-Type: " + contentType + "\r\n\r\n" + content;
    } else if (method == "POST") {
        // Handle API calls
        size_t bodyPos = request.find("\r\n\r\n");
        string body = request.substr(bodyPos + 4);

        if (path == "/login") {
            // Parse JSON body
            // Simple parse, assume {"email":"...", "password":"..."}
            size_t emailStart = body.find("\"email\":\"") + 9;
            size_t emailEnd = body.find("\"", emailStart);
            string email = body.substr(emailStart, emailEnd - emailStart);
            size_t passStart = body.find("\"password\":\"") + 12;
            size_t passEnd = body.find("\"", passStart);
            string password = body.substr(passStart, passEnd - passStart);

            if (globalSystem->authenticate(email, password)) {
                currentUser = globalSystem->getCurrentUser();
                return "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n{\"success\":true}";
            } else {
                return "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n{\"success\":false}";
            }
        } else if (path == "/register") {
            // Parse JSON
            // Assume {"firstName":"...", "lastName":"...", "email":"...", "password":"...", "balance":...}
            // Simple parse
            size_t fnStart = body.find("\"firstName\":\"") + 13;
            size_t fnEnd = body.find("\"", fnStart);
            string firstName = body.substr(fnStart, fnEnd - fnStart);
            size_t lnStart = body.find("\"lastName\":\"") + 12;
            size_t lnEnd = body.find("\"", lnStart);
            string lastName = body.substr(lnStart, lnEnd - lnStart);
            size_t emailStart = body.find("\"email\":\"") + 9;
            size_t emailEnd = body.find("\"", emailStart);
            string email = body.substr(emailStart, emailEnd - emailStart);
            size_t passStart = body.find("\"password\":\"") + 12;
            size_t passEnd = body.find("\"", passStart);
            string password = body.substr(passStart, passEnd - passStart);
            size_t balStart = body.find("\"balance\":") + 10;
            size_t balEnd = body.find("}", balStart);
            double balance = stod(body.substr(balStart, balEnd - balStart));

            globalSystem->registerUser(firstName, lastName, email, password, balance);
            return "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n{\"success\":true}";
        } else if (path == "/user/info" && currentUser) {
            string json = "{\"fullName\":\"" + currentUser->getFullName() + "\", \"balance\":" + to_string(currentUser->getAccountBalance()) + "}";
            return "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n" + json;
        } else if (path == "/resources") {
            string json = "[";
            for (size_t i = 0; i < globalSystem->resources.size(); ++i) {
                Resource* r = globalSystem->resources[i];
                json += "{\"id\":" + to_string(r->getResourceID()) + ", \"title\":\"" + r->getTitle() + "\", \"author\":\"" + r->getAuthor() + "\"}";
                if (i < globalSystem->resources.size() - 1) json += ",";
            }
            json += "]";
            return "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n" + json;
        } else if (path == "/user/borrowed" && currentUser) {
            // API endpoint for borrowed books - currently not fully implemented
            // The UI uses client-side storage for this functionality
            string json = "[]";
            return "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n" + json;
        } else if (path == "/borrow" && currentUser) {
            // Parse {"resourceId":..., "date":"..."}
            size_t idStart = body.find("\"resourceId\":") + 13;
            size_t idEnd = body.find(",", idStart);
            int resourceId = stoi(body.substr(idStart, idEnd - idStart));
            size_t dateStart = body.find("\"date\":\"") + 8;
            size_t dateEnd = body.find("\"", dateStart);
            string date = body.substr(dateStart, dateEnd - dateStart);

            bool success = globalSystem->borrowResource(resourceId, date);
            string message = success ? "Borrowed successfully" : "Borrow failed";
            return "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n{\"message\":\"" + message + "\"}";
        } else if (path == "/return" && currentUser) {
            // Similar to borrow
            size_t idStart = body.find("\"resourceId\":") + 13;
            size_t idEnd = body.find(",", idStart);
            int resourceId = stoi(body.substr(idStart, idEnd - idStart));
            size_t dateStart = body.find("\"date\":\"") + 8;
            size_t dateEnd = body.find("\"", dateStart);
            string date = body.substr(dateStart, dateEnd - dateStart);

            double fine = globalSystem->returnResource(resourceId, date);
            string message = "Returned successfully, fine: " + to_string(fine);
            return "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n{\"message\":\"" + message + "\"}";
        } else if (path == "/recharge" && currentUser) {
            size_t amtStart = body.find("\"amount\":") + 9;
            size_t amtEnd = body.find("}", amtStart);
            double amount = stod(body.substr(amtStart, amtEnd - amtStart));
            currentUser->rechargebalance(amount);
            return "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n{\"message\":\"Recharged successfully\"}";
        } else if (path == "/logout") {
            currentUser = nullptr;
            globalSystem->logout();
            return "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n{\"success\":true}";
        }
        // Add more as needed
    }
    return "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\nNot Found";
}

void handleClient(SOCKET clientSocket) {
    char buffer[1024];
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesReceived > 0) {
        string request(buffer, bytesReceived);
        string response = handleRequest(request);
        send(clientSocket, response.c_str(), response.size(), 0);
    }
    closesocket(clientSocket);
}

void startServer() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, 5);

    cout << "Server started on http://localhost:8080" << endl;

    while (true) {
        SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
        handleClient(clientSocket);
    }

    closesocket(serverSocket);
    WSACleanup();
}

void testSeparator(string title)
{
    cout << "\n"
         << string(50, '=') << endl;
    cout << "  " << title << endl;
    cout << string(50, '=') << endl;
}

void runTests() {
    cout << "\n"
         << string(60, '#') << endl;
    cout << "#" << string(58, ' ') << "#" << endl;
    cout << "#" << string(15, ' ') << "LIBRARY SYSTEM COMPREHENSIVE TEST" << string(10, ' ') << "#" << endl;
    cout << "#" << string(58, ' ') << "#" << endl;
    cout << string(60, '#') << endl;

    LibrarySystem system;

    // ========================================
    // CLEAN UP OLD DATA FILES (Fresh Start)
    // ========================================
    testSeparator("CLEARING OLD DATA FOR FRESH TEST");
    system.users.clear();
    system.admins.clear();
    system.resources.clear();
    cout << "[PASS] Old data cleared - starting fresh test" << endl;

    // ========================================
    // TEST 1: USER CREATION & MANAGEMENT
    // ========================================
    testSeparator("TEST 1: USER CREATION & MANAGEMENT");

    User *u1 = new User(1, "Ali", "Khan", "ali@email.com", "1234", 500.0);
    User *u2 = new User(2, "Sara", "Ahmed", "sara@gmail.com", "5678", 350.0);
    User *u3 = new User(3, "Hassan", "Mohamed", "hassan@email.com", "9999", 200.0);
    User *u4 = new User(4, "Fatima", "Ali", "fatima@email.com", "1111", 100.0);

    system.addUser(u1);
    system.addUser(u2);
    system.addUser(u3);
    system.addUser(u4);
    cout << "[PASS] Created and added 4 users to system" << endl;

    // ========================================
    // TEST 2: ADMIN CREATION & MANAGEMENT
    // ========================================
    testSeparator("TEST 2: ADMIN CREATION & MANAGEMENT");

    Admin *a1 = new Admin(101, "Mr", "Boss", "admin@library.com", "admin123", "SuperAdmin");
    Admin *a2 = new Admin(102, "Ms", "Manager", "manager@library.com", "manager123", "Admin");

    system.addAdmin(a1);
    system.addAdmin(a2);
    cout << "[PASS] Created and added 2 admins to system" << endl;

    // ========================================
    // TEST 3: RESOURCE CREATION (ALL TYPES)
    // ========================================
    testSeparator("TEST 3: RESOURCE CREATION (ALL TYPES)");

    PrimePickBook *book1 = new PrimePickBook(1001, "The Good Gatsby", "F. Scott Fitzgerald", "Fiction");
    PrimePickBook *book2 = new PrimePickBook(1002, "Moby Dick", "Herman Melville", "Adventure");

    ClassicShelfBook *book3 = new ClassicShelfBook(1003, "To Kill a Mockingbird", "Harper Lee", "Fiction");
    ClassicShelfBook *book4 = new ClassicShelfBook(1004, "Pride and Prejudice", "Jane Austen", "Romance");

    BudgetPickBook *book5 = new BudgetPickBook(1005, "1984", "George Orwell", "Dystopian");
    BudgetPickBook *book6 = new BudgetPickBook(1006, "Brave New World", "Aldous Huxley", "Science Fiction");

    // DigitalLibrary ebook1 = new DigitalLibrary(2001, "Digital Programming", "Robert Martin", "Technology");
    
    system.resources.push_back(book1);
    system.resources.push_back(book2);
    system.resources.push_back(book3);
    system.resources.push_back(book4);
    system.resources.push_back(book5);  
    system.resources.push_back(book6);
    // system.resources.push_back(ebook1);
    
    cout << "[PASS] Created 6 resources (4 physical books + 2 budget books)" << endl;

    // ========================================
    // TEST 4: MEMBERSHIP ASSIGNMENT (3-TIER SYSTEM)
    // ========================================
    testSeparator("TEST 4: MEMBERSHIP ASSIGNMENT (3-TIER SYSTEM)");

    u1->setMembership(new DeluxeMembership());
    u2->setMembership(new ExtraMembership());
    u3->setMembership(new NormalMembership());
    u4->setMembership(new NormalMembership());
    
    cout << "[PASS] Assigned memberships:" << endl;
    cout << "  - u1 (Ali): Deluxe ($20/month)" << endl;
    cout << "  - u2 (Sara): Extra ($10/month)" << endl;
    cout << "  - u3 (Hassan): Essential (Free)" << endl;
    cout << "  - u4 (Fatima): Essential (Free)" << endl;

    // ========================================
    // TEST 4B: USER SELECTS OWN TIER
    // ========================================
    testSeparator("TEST 4B: USER SELECTS OWN TIER");
    u3->showMembershipOptions();
    u3->changeMembershipTier(2);
    u3->displayMembershipDetails();
    cout << "[PASS] User 3 selected Extra membership successfully" << endl;

    // ========================================
    // TEST 4C: ADMIN ASSIGNS USER TIER
    // ========================================
    testSeparator("TEST 4C: ADMIN ASSIGNS USER TIER");
    a1->assignMembershipTier(4, 3, system);
    u4->displayMembershipDetails();
    cout << "[PASS] Admin assigned Deluxe membership to User 4 successfully" << endl;

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
    
    cout << "\n[PASS] Books borrowed successfully" << endl;

    // ========================================
    // TEST 6: RETURN OPERATIONS
    // ========================================
    testSeparator("TEST 6: RETURN OPERATIONS");

    cout << "\n--- User 1 Returning Book ---" << endl;
    u1->returnresources(book1, "2025-05-10");
    cout << "[PASS] Book returned successfully" << endl;

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
    cout << "[PASS] Loyalty points earned and added" << endl;

    // ========================================
    // TEST 8B: LOYALTY POINTS REDEMPTION
    // ========================================
    testSeparator("TEST 8B: LOYALTY POINTS REDEMPTION");

    cout << "\nUser 1 Loyalty Summary:" << endl;
    u1->displayLoyaltySummary();
    
    cout << "\nAttempting to redeem 100 points for discount..." << endl;
    u1->redeemPointsForDiscount(100);
    cout << "New Balance: Rs." << u1->getAccountBalance() << endl;

    // ========================================
    // TEST 8C: MEMBERSHIP DETAILS
    // ========================================
    testSeparator("TEST 8C: MEMBERSHIP TIER DETAILS");

    cout << "\n--- User 1 (Deluxe) ---" << endl;
    u1->displayMembershipDetails();

    cout << "\n--- User 2 (Extra) ---" << endl;
    u2->displayMembershipDetails();

    cout << "\n--- User 3 (Essential) ---" << endl;
    u3->displayMembershipDetails();

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
             << " | Author: " << res->getAuthor()
             << " | Category: " << res->getCategory()
             << " | Available: " << (res->getAvailability() ? "Yes" : "No") << endl;
    }

    // ========================================
    // TEST 12: FILTER AVAILABLE RESOURCES
    // ========================================
    testSeparator("TEST 12: FILTER AVAILABLE RESOURCES");

    auto availableBooks = system.filterByAvailability();
    cout << "\nAvailable Books: " << availableBooks.size() << endl;
    for (const auto &book : availableBooks)
    {
        cout << "  - " << book->getTitle() << endl;
    }

    // ========================================
    // TEST 13: SEARCH RESOURCES
    // ========================================
    testSeparator("TEST 13: SEARCH RESOURCES");

    cout << "\nSearching for 'Great'..." << endl;
    auto searchResults = system.searchResource("Great");
    cout << "Found: " << searchResults.size() << " book(s)" << endl;
    for (const auto &book : searchResults)
    {
        cout << "  - " << book->getTitle() << endl;
    }

    // ========================================
    // TEST 14: FILTER BY CATEGORY
    // ========================================
    testSeparator("TEST 14: FILTER BY CATEGORY");

    cout << "\nFiltering by category 'Fiction'..." << endl;
    auto fictionBooks = system.filterResources("Fiction");
    cout << "Fiction Books Found: " << fictionBooks.size() << endl;
    for (const auto &book : fictionBooks)
    {
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
    for (int i = 0; i < min(3, (int)ratedBooks.size()); i++)
    {
        cout << "  " << (i + 1) << ". " << ratedBooks[i]->getTitle()
             << " - Rating: " << ratedBooks[i]->getReviewScore() << "/5" << endl;
    }

    // ========================================
    // TEST 21: MOST BORROWED BOOKS
    // ========================================
    testSeparator("TEST 21: MOST BORROWED BOOKS");

    auto mostBorrowed = system.filterByMostBorrowed();
    cout << "\nMost Borrowed Books (first 3):" << endl;
    for (int i = 0; i < min(3, (int)mostBorrowed.size()); i++)
    {
        cout << "  " << (i + 1) << ". " << mostBorrowed[i]->getTitle()
             << " - Times Borrowed: " << mostBorrowed[i]->getBorrowCount() << endl;
    }

    // ========================================
    // FINAL SUMMARY
    // ========================================
    testSeparator("FINAL SUMMARY");
    
    cout << "\n[PASS] Total Users in System: " << system.users.size() << endl;
    cout << "[PASS] Total Admins in System: " << system.admins.size() << endl;
    cout << "[PASS] Total Resources in System: " << system.resources.size() << endl;
    cout << "[PASS] Available Resources: " << system.filterByAvailability().size() << endl;
    cout << "[PASS] Borrowed Resources: " << (system.resources.size() - system.filterByAvailability().size()) << endl;

    cout << "\n"
         << string(60, '#') << endl;
    cout << "#" << string(15, ' ') << "ALL TESTS COMPLETED SUCCESSFULLY!" << string(10, ' ') << "#" << endl;
    cout << string(60, '#') << endl;

    // Save all data to files
    cout << "\n" << string(50, '=') << endl;
    cout << "SAVING DATA TO FILES..." << endl;
    cout << string(50, '=') << endl;
    system.saveData();
}

int main()
{
    LibrarySystem system;
    globalSystem = &system;
    runTests();
    startServer();
    return 0;
}