#include "NotificationService.h"
#include "../users/user.h"
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
    if (!user)
    {
        cerr << "Error: null user." << endl;
        return;
    }
    string msg = "REMINDER: Your borrowed items are due soon.";
    logNotification(user->getEmail(), msg);
    cout << "[DUE REMINDER] To: " << user->getFullName() << " | " << msg << endl;
}

void NotificationService::sendOverdueAlert(User *user)
{
    if (!user)
    {
        cerr << "Error: null user." << endl;
        return;
    }
    string msg = "ALERT: Your items are overdue! Late fees applied.";
    logNotification(user->getEmail(), msg);
    cout << "[OVERDUE ALERT] To: " << user->getFullName() << " | " << msg << endl;
}

void NotificationService::sendWishlistAlert(User *user)
{
    if (!user)
    {
        cerr << "Error: null user." << endl;
        return;
    }
    string msg = "Good news! A wishlist item is now available.";
    logNotification(user->getEmail(), msg);
    cout << "[WISHLIST ALERT] To: " << user->getFullName() << " | " << msg << endl;
}
void NotificationService::logNotification(const string &userEmail, const string &message)
{
    // Getting current time:
    time_t now = time(0);
    char *dt = ctime(&now);
    dt[strlen(dt) - 1] = '\0'; /*remove new line*/
    cout << "[LOG -" << dt << "] Notification to " << userEmail << ": " << message << endl;
}