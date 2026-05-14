#ifndef RESERVATION_H
#define RESERVATION_H

#include <string>
#include <ctime>
using namespace std;

class User;

class Reservation {
private:
    User* user;
    int resourceID;
    string resourceName;
    string reservationDate;
    string status;  // "pending", "cancelled", "fulfilled"
    int positionInQueue;

public:
    Reservation(User* user, int resourceID, string resourceName, string reservationDate, int queuePosition);

    // Getters
    int getUserID() const;
    int getResourceID() const;
    string getResourceName() const;
    string getReservationDate() const;
    string getStatus() const;
    int getQueuePosition() const;
    User* getUser() const;

    // Setters
    void setStatus(string newStatus);
    void setQueuePosition(int newPosition);

    // Display
    void showReservation() const;
};

#endif
