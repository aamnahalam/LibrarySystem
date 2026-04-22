#ifndef NOTIFICATIONSERVICE_H
#define NOTIFICATIONSERVICE_H

#include <string>
#include <iostream>
using namespace std;

// Forward declaration:
class User;

class NotificationService
{
public:
    // Constructor:
    NotificationService();
    // Destructor:
    ~NotificationService();

    // Methods:
    void sendDueReminder(User *user);
    void sendOverdueAlert(User *user);
    void sendWishlistAlert(User *user);

private:
    // A private method
    void logNotification(const string &userEmail, const string &message);
};

#endif