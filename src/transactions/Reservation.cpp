#include "Reservation.h"
#include "../users/user.h"
#include <iostream>
#include <iomanip>
using namespace std;

Reservation::Reservation(User* user, int resourceID, string resourceName, string reservationDate, int queuePosition)
{
    this->user = user;
    this->resourceID = resourceID;
    this->resourceName = resourceName;
    this->reservationDate = reservationDate;
    this->status = "pending";
    this->positionInQueue = queuePosition;
}

int Reservation::getUserID() const
{
    if (user == nullptr) return -1;
    return user->getID();
}

int Reservation::getResourceID() const
{
    return resourceID;
}

string Reservation::getResourceName() const
{
    return resourceName;
}

string Reservation::getReservationDate() const
{
    return reservationDate;
}

string Reservation::getStatus() const
{
    return status;
}

int Reservation::getQueuePosition() const
{
    return positionInQueue;
}

User* Reservation::getUser() const
{
    return user;
}

void Reservation::setStatus(string newStatus)
{
    this->status = newStatus;
}

void Reservation::setQueuePosition(int newPosition)
{
    this->positionInQueue = newPosition;
}

void Reservation::showReservation() const
{
    cout << left << setw(10) << positionInQueue
         << setw(25) << resourceName.substr(0, 24)
         << setw(15) << reservationDate
         << setw(12) << status << "\n";
}
