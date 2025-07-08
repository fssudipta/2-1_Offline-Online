#include <iostream>
#include <unordered_map>
#include <string>
using namespace std;

struct Citizen {
    string name;
    string nid;
    string phone;
};

unordered_map<string, Citizen> nameIndex;
unordered_map<string, Citizen> nidIndex;
unordered_map<string, Citizen> phoneIndex;

void insertCitizen() {
    Citizen c;
    cout << "Enter name: ";
    getline(cin >> ws, c.name);
    cout << "Enter NID no: ";
    getline(cin, c.nid);
    cout << "Enter phone no: ";
    getline(cin, c.phone);

    // Insert into all indexes
    nameIndex[c.name] = c;
    nidIndex[c.nid] = c;
    phoneIndex[c.phone] = c;
}

void searchCitizen() {
    string query;
    cout << "Enter search string: ";
    getline(cin >> ws, query);

    if (nameIndex.count(query)) {
        Citizen c = nameIndex[query];
        cout << "Match Found:\n";
        cout << "Name: " << c.name << "\n";
        cout << "NID no: " << c.nid << "\n";
        cout << "Phone no: " << c.phone << "\n";
    } else if (nidIndex.count(query)) {
        Citizen c = nidIndex[query];
        cout << "Match Found:\n";
        cout << "Name: " << c.name << "\n";
        cout << "NID no: " << c.nid << "\n";
        cout << "Phone no: " << c.phone << "\n";
    } else if (phoneIndex.count(query)) {
        Citizen c = phoneIndex[query];
        cout << "Match Found:\n";
        cout << "Name: " << c.name << "\n";
        cout << "NID no: " << c.nid << "\n";
        cout << "Phone no: " << c.phone << "\n";
    } else {
        cout << "No match found\n";
    }
}

int main() {
    int choice;

    while (true) {
        cout << "Enter choice: 1. Enter Data, 2. Search Index, 3. Exit\n";
        cin >> choice;
        if (choice == 1) {
            insertCitizen();
        } else if (choice == 2) {
            searchCitizen();
        } else if (choice == 3) {
            break;
        } else {
            cout << "Invalid choice\n";
        }
    }

    return 0;
}
