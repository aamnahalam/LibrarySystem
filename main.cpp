#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <climits>
#include <cctype>
#include <ctime>
#include <iomanip>
#include <sstream>
#include "src/core/LibrarySystem.h"
#include "src/users/user.h"
#include "src/users/admin.h"
#include "src/resources/PrimePickBook.h"
#include "src/resources/ClassicShelfBook.h"
#include "src/resources/BudgetPickBook.h"
#include "src/Membership/NormalMembership.h"
#include "src/Membership/ExtraMembership.h"
#include "src/Membership/DeluxeMembership.h"

using namespace std;

void pressEnterToContinue()
{
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int promptInt(const string &prompt, int minValue = INT_MIN, int maxValue = INT_MAX)
{
    while (true) {
        cout << prompt;
        int value;
        if (!(cin >> value)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a valid number.\n";
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (value < minValue || value > maxValue) {
            cout << "Please enter a value between " << minValue << " and " << maxValue << ".\n";
            continue;
        }
        return value;
    }
}

double promptDouble(const string &prompt)
{
    while (true) {
        cout << prompt;
        double value;
        if (!(cin >> value)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a valid number.\n";
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return value;
    }
}

string promptLine(const string &prompt)
{
    string line;
    cout << prompt;
    getline(cin, line);
    return line;
}

bool isValidDateFormat(const string &date)
{
    if (date.size() != 10 || date[4] != '-' || date[7] != '-')
        return false;
    for (int i = 0; i < 10; ++i) {
        if (i == 4 || i == 7)
            continue;
        if (!isdigit(static_cast<unsigned char>(date[i])))
            return false;
    }
    return true;
}

string getCurrentDateString()
{
    time_t now = time(0);
    tm *ltm = localtime(&now);
    stringstream ss;
    ss << (1900 + ltm->tm_year) << "-" << setfill('0') << setw(2) << (1 + ltm->tm_mon) << "-" << setfill('0') << setw(2) << ltm->tm_mday;
    return ss.str();
}

void displayResourceList(const vector<Resource *> &resources)
{
    if (resources.empty()) {
        cout << "No resources found.\n";
        return;
    }

    for (const Resource *res : resources) {
        cout << "ID: " << res->getResourceID()
             << " | Title: " << res->getTitle()
             << " | Author: " << res->getAuthor()
             << " | Category: " << res->getCategory()
             << " | Available: " << (res->getAvailability() ? "Yes" : "No") << "\n";
    }
}

void seedSystem(LibrarySystem &system)
{
    // Add default admin if not already present
    bool adminExists = false;
    for (auto *a : system.admins) {
        if (a->getEmail() == "admin@library.com") {
            adminExists = true;
            break;
        }
    }
    if (!adminExists) {
        Admin *admin = new Admin(1001, "Super", "Admin", "admin@library.com", "admin123", "SuperAdmin");
        system.addAdmin(admin);
    }

    if (system.resources.empty()) {
        system.resources.push_back(new PrimePickBook(2001, "The Great Gatsby", "F. Scott Fitzgerald", "Fiction"));
        system.resources.push_back(new PrimePickBook(2002, "Moby Dick", "Herman Melville", "Adventure"));
        system.resources.push_back(new ClassicShelfBook(2003, "To Kill a Mockingbird", "Harper Lee", "Fiction"));
        system.resources.push_back(new ClassicShelfBook(2004, "Pride and Prejudice", "Jane Austen", "Romance"));
        system.resources.push_back(new BudgetPickBook(2005, "1984", "George Orwell", "Dystopian"));
        system.resources.push_back(new BudgetPickBook(2006, "Brave New World", "Aldous Huxley", "Science Fiction"));
    }
}

bool attemptLogin(LibrarySystem &system, bool requireAdmin)
{
    string email = promptLine("Email: ");
    string password = promptLine("Password: ");

    if (!system.authenticate(email, password)) {
        cout << "Login failed. Please check email and password.\n";
        return false;
    }

    if (requireAdmin) {
        if (system.getCurrentAdmin() == nullptr) {
            cout << "This account is not an admin.\n";
            system.logout();
            return false;
        }
        cout << "Admin login successful.\n";
        return true;
    }

    if (system.getCurrentUser() == nullptr) {
        cout << "This account is not a standard user.\n";
        system.logout();
        return false;
    }

    cout << "User login successful.\n";
    return true;
}

void userMenu(LibrarySystem &system)
{
    User *user = system.getCurrentUser();
    if (!user) {
        return;
    }

    while (true) {
        cout << "\n===== User Menu =====\n";
        cout << "1) View All Resources\n";
        cout << "2) Search Resources\n";
        cout << "3) Borrow Resource\n";
        cout << "4) Return Resource\n";
        cout << "5) View Profile and Borrow History\n";
        cout << "6) Recharge Balance\n";
        cout << "7) Rate a Resource\n";
        cout << "8) Logout\n";

        int choice = promptInt("Choose an option: ", 1, 8);

        if (choice == 1) {
            cout << "\n--- All Resources ---\n";
            displayResourceList(system.resources);
            pressEnterToContinue();
        } else if (choice == 2) {
            string keyword = promptLine("Enter title or category keyword: ");
            auto results = system.searchResource(keyword);
            cout << "\n--- Search Results ---\n";
            displayResourceList(results);
            pressEnterToContinue();
        } else if (choice == 3) {
            int resourceId = promptInt("Enter resource ID to borrow: ", 1, INT_MAX);
            string date = getCurrentDateString();
            if (system.borrowResource(resourceId, date)) {
                cout << "Borrow request succeeded.\n";
            } else {
                cout << "Failed to borrow the selected resource.\n";
            }
            pressEnterToContinue();
        } else if (choice == 4) {
            int resourceId = promptInt("Enter resource ID to return: ", 1, INT_MAX);
            string date = getCurrentDateString();
            double fine = system.returnResource(resourceId, date);
            if (fine >= 0.0)
                cout << "Return completed. Fine charged: " << fine << "\n";
            pressEnterToContinue();
        } else if (choice == 5) {
            cout << "\n--- Profile Information ---\n";
            user->displayInfo();
            cout << "\n--- Borrow History ---\n";
            user->viewhistory();
            pressEnterToContinue();
        } else if (choice == 6) {
            double amount = promptDouble("Enter recharge amount: ");
            user->rechargebalance(amount);
            cout << "Balance updated. New balance: " << user->getAccountBalance() << "\n";
            pressEnterToContinue();
        } else if (choice == 7) {
            int resourceId = promptInt("Enter resource ID to rate (must have borrowed it): ", 1, INT_MAX);
            Resource *res = system.getResourceByID(resourceId);
            if (!res) {
                cout << "Resource not found.\n";
            } else {
                // Check if user has borrowed this resource
                bool hasBorrowed = false;
                for (auto &record : user->getBorrowHistory()) {
                    if (record.getResourceID() == resourceId) {
                        hasBorrowed = true;
                        break;
                    }
                }
                if (!hasBorrowed) {
                    cout << "You have not borrowed this resource.\n";
                } else {
                    int rating = promptInt("Enter rating (1-5): ", 1, 5);
                    res->addRating(rating);
                    cout << "Rating added successfully.\n";
                }
            }
            pressEnterToContinue();
        } else {
            system.logout();
            cout << "Logged out.\n";
            break;
        }
    }
}

void addResourceInteractive(Admin *admin, LibrarySystem &system)
{
    cout << "\nChoose resource type to add:\n";
    cout << "1) PrimePick Book\n";
    cout << "2) ClassicShelf Book\n";
    cout << "3) BudgetPick Book\n";
    int typeChoice = promptInt("Select type: ", 1, 3);
    int resourceId = promptInt("Enter resource ID: ", 1, INT_MAX);
    string title = promptLine("Enter title: ");
    string author = promptLine("Enter author: ");
    string category = promptLine("Enter category: ");

    Resource *resource = nullptr;
    if (typeChoice == 1) {
        resource = new PrimePickBook(resourceId, title, author, category);
    } else if (typeChoice == 2) {
        resource = new ClassicShelfBook(resourceId, title, author, category);
    } else {
        resource = new BudgetPickBook(resourceId, title, author, category);
    }

    admin->addResources(resource, system);
    cout << "Resource added successfully.\n";
    pressEnterToContinue();
}

void createAdminInteractive(Admin *admin, LibrarySystem &system)
{
    string firstName = promptLine("Admin first name: ");
    string lastName = promptLine("Admin last name: ");
    string email = promptLine("Admin email: ");
    string password = promptLine("Admin password: ");
    string level = promptLine("Access level (Admin or SuperAdmin): ");

    if (admin->createAdmin(firstName, lastName, email, password, level, system)) {
        cout << "Admin account created successfully.\n";
    } else {
        cout << "Failed to create admin account.\n";
    }
    pressEnterToContinue();
}

void adminMenu(LibrarySystem &system)
{
    Admin *admin = system.getCurrentAdmin();
    if (!admin) {
        return;
    }

    while (true) {
        cout << "\n===== Admin Menu =====\n";
        cout << "1) View All Users\n";
        cout << "2) View All Resources\n";
        cout << "3) Add Resource\n";
        cout << "4) Remove Resource\n";
        cout << "5) Lock User\n";
        cout << "6) View Issued Resources Report\n";
        cout << "7) View Overdue Resources Report\n";
        if (admin->getAccessLevel() == "SuperAdmin") {
            cout << "8) Create Admin Account\n";
            cout << "9) Logout\n";
        } else {
            cout << "8) Logout\n";
        }

        int maxChoice = (admin->getAccessLevel() == "SuperAdmin") ? 9 : 8;
        int choice = promptInt("Choose an option: ", 1, maxChoice);

        if (choice == 1) {
            cout << "\n--- Registered Users ---\n";
            system.showAllUsers();
            pressEnterToContinue();
        } else if (choice == 2) {
            cout << "\n--- Library Resources ---\n";
            displayResourceList(system.resources);
            pressEnterToContinue();
        } else if (choice == 3) {
            addResourceInteractive(admin, system);
        } else if (choice == 4) {
            int resourceId = promptInt("Enter resource ID to remove: ", 1, INT_MAX);
            if (admin->removeResources(resourceId, system)) {
                cout << "Resource removed successfully.\n";
            } else {
                cout << "Failed to remove resource.\n";
            }
            pressEnterToContinue();
        } else if (choice == 5) {
            int userId = promptInt("Enter user ID to lock: ", 1, INT_MAX);
            admin->lockUser(userId, system);
            cout << "Lock operation attempted.\n";
            pressEnterToContinue();
        } else if (choice == 6) {
            cout << "\n--- Issued Resources Report ---\n";
            admin->generateIssuedResourcesReport(system);
            pressEnterToContinue();
        } else if (choice == 7) {
            cout << "\n--- Overdue Resources Report ---\n";
            admin->generateOverdueResourcesReport(system);
            pressEnterToContinue();
        } else if (choice == 8 && admin->getAccessLevel() == "SuperAdmin") {
            createAdminInteractive(admin, system);
        } else {
            system.logout();
            cout << "Admin logged out.\n";
            break;
        }
    }
}

void registerUserInteractive(LibrarySystem &system)
{
    string firstName = promptLine("First name: ");
    string lastName = promptLine("Last name: ");
    string email = promptLine("Email: ");
    string password = promptLine("Password: ");
    double balance = promptDouble("Initial balance: ");

    system.registerUser(firstName, lastName, email, password, balance);
    cout << "Registration complete. You may now login.\n";
    pressEnterToContinue();
}

int main()
{
    LibrarySystem system;
    system.loadData();
    seedSystem(system);

    while (true) {
        cout << "\n===== Library System Menu =====\n";
        cout << "1) Login as User\n";
        cout << "2) Login as Admin\n";
        cout << "3) Register New User\n";
        cout << "4) Exit\n";

        int choice = promptInt("Choose an option: ", 1, 4);

        if (choice == 1) {
            if (attemptLogin(system, false)) {
                userMenu(system);
            }
        } else if (choice == 2) {
            if (attemptLogin(system, true)) {
                adminMenu(system);
            }
        } else if (choice == 3) {
            registerUserInteractive(system);
        } else {
            system.saveData();
            cout << "Exiting the library system. Goodbye!\n";
            break;
        }
    }

    return 0;
}
