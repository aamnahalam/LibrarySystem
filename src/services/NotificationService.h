#ifndef NOTIFICATIONSERVICE_H
#define NOTIFICATIONSERVICE_H

#include <string>

class NotificationService
{
public:
    NotificationService();

    void sendNotification(const std::string& userId, const std::string& message);
    void sendEmail(const std::string& email, const std::string& message);

private:
};

#endif