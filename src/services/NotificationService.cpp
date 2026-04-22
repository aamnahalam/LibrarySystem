#include "NotificationService.h"
#include "user.h"
#include <iostream>
#include <ctime>
#include <cstring>

NotificationService::NotificationService()
{
    cout << "Notification service initialized." << endl;
}

NotificationService::~NotificationService()
{
    cout << "Notification service shutting down." << endl;
}

void NotificationService::sendDueReminder(User *user)
{
    if (user == nullptr)
    {
        cerr << "Error: Cannot send due reminder to null user." << endl;
        return;
    }
    string message = "REMINDER: Your borrowed items are due soon. Please return them to avoid late fees.";
    logNotification(user->getEmail(), message);

    cout << "[DUE REMINDER] Sent to: " << user->getFullName() << " (" << user->getEmail() << ")" << endl;
    cout << " Message: " << message << endl;
}

void NotificationService::sendOverdueAlert(User *user)
{
    if (user == nullptr)
    {
        cerr << "Error: Cannot send overdue alert to null user." << endl;
        return;
    }
    string message = "ALERT: Your borrowed items are overdue! Late fees have been applied. Please return immediately.";
    logNotification(user->getEmail(), message);

    cout << "[OVERDUE ALERT] Sent to" << user->getFullName() << " (" << user->getEmail() << ")" << endl;
    cout << " Message: " << message << endl;
}

void NotificationService::sendWishlistAlert(User *user)
{
    if (user == nullptr)
    {
        cerr << "Error: Cannot send wishlist alert to null user." << endl;
        return;
    }
    string message = "Good news! An item from your wishlist is now available.";
    logNotification(user->getEmail(), message);

    cout << "[Wishlist Alert] Sent to" << user->getFullName() << " (" << user->getEmail() << ")" << endl;
    cout << " Message: " << message << endl;
}

void NotificationService::logNotification(const string &userEmail, const string &message)
{
    // Getting current time:
    time_t now = time(0);
    char *dt = ctime(&now);
    dt[strlen(dt) - 1] = '\0'; /*remove new line*/
    // This would write to a database or logfile:
    cout << "[LOG -" << dt << "] Notification to " << userEmail << ": " << message << endl;
}