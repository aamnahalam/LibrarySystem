#ifndef CATEGORYPREFERENCE_H
#define CATEGORYPREFERENCE_H

#include <string>
using namespace std;

class CategoryPreference {
private:
    string category;

public:
    CategoryPreference();
    CategoryPreference(string category);

    void setCategory(string category);
    string getCategory() const;

    bool isValidCategory(string category);   // validation
    void showPreference() const;
};

#endif