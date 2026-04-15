#ifndef RESOURCE_H
#define RESOURCE_H

#include <string>
using namespace std;

class Resource {
protected:
    int id;
    string title;
    bool available;

public:
    Resource(int id, string title);

    virtual void display();
    bool isAvailable();
    void borrow();
    void giveBack();
};

#endif