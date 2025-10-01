#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cstdlib>

using namespace std;

struct Contact {
    string name;
    string phone;
    Contact* next;
    Contact* prev;
};

class ContactBook {
private:
    Contact* head;
    Contact* tail;

    void sortAlphabetically() {
        if (!head) return;
        bool swapped;
        do {
            swapped = false;
            Contact* current = head;
            while (current->next) {
                if (current->name > current->next->name) {
                    swap(current->name, current->next->name);
                    swap(current->phone, current->next->phone);
                    swapped = true;
                }
                current = current->next;
            }
        } while (swapped);
    }

    void saveToFile() {
        ofstream file("contacts.txt", ios::trunc);
        if (file.is_open()) {
            Contact* current = head;
            while (current) {
                file << current->name << endl;
                file << current->phone << endl;
                current = current->next;
            }
            file.close();
        } else {
            cout << "Unable to open file for saving contacts." << endl;
        }
    }

    void loadFromFile() {
        ifstream file("contacts.txt");
        if (file.is_open()) {
            string name, phone;
            while (getline(file, name) && getline(file, phone)) {
                addContact(name, phone);
            }
            file.close();
        } else {
            cout << "No previous contacts found." << endl;
        }
    }

public:
    ContactBook() : head(nullptr), tail(nullptr) {
        loadFromFile();
    }

    ~ContactBook() {
        Contact* current = head;
        while (current) {
            Contact* toDelete = current;
            current = current->next;
            delete toDelete;
        }
    }

    void addContact(const string& name, const string& phone) {
        Contact* newContact = new Contact{name, phone, nullptr, nullptr};
        if (!head) {
            head = tail = newContact;
        } else {
            tail->next = newContact;
            newContact->prev = tail;
            tail = newContact;
        }
        sortAlphabetically();
        saveToFile();
    }

    void displayContacts() const {
        if (!head) {
            cout << "No contacts available." << endl;
            return;
        }
        Contact* current = head;
        while (current) {
            cout << left << setw(20) << current->name << setw(15) << current->phone << endl;
            current = current->next;
        }
    }

    void searchContact(const string& keyword) const {
        Contact* current = head;
        bool found = false;
        while (current) {
            if (current->name.find(keyword) != string::npos || current->phone.find(keyword) != string::npos) {
                cout << left << setw(20) << current->name << setw(15) << current->phone << endl;
                found = true;
            }
            current = current->next;
        }
        if (!found) {
            cout << "No contact found with the given keyword." << endl;
        }
    }

    void deleteContact(const string& name) {
        Contact* current = head;
        while (current) {
            if (current->name == name) {
                if (current->prev) current->prev->next = current->next;
                else head = current->next;
                if (current->next) current->next->prev = current->prev;
                else tail = current->prev;
                delete current;
                saveToFile();
                cout << "Contact deleted successfully." << endl;
                return;
            }
            current = current->next;
        }
        cout << "Contact not found." << endl;
    }
};

int main() {
    ContactBook book;
    int choice;
    string name, phone, keyword;

    do {
        cout << "\nContact Book Menu:\n";
        cout << "1. Add Contact\n";
        cout << "2. Display Contacts\n";
        cout << "3. Search Contact\n";
        cout << "4. Delete Contact\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1:
                cout << "Enter name: ";
                getline(cin, name);
                cout << "Enter phone number: ";
                getline(cin, phone);
                book.addContact(name, phone);
                break;
            case 2:
                book.displayContacts();
                break;
            case 3:
                cout << "Enter search keyword: ";
                getline(cin, keyword);
                book.searchContact(keyword);
                break;
            case 4:
                cout << "Enter name of contact to delete: ";
                getline(cin, name);
                book.deleteContact(name);
                break;
            case 5:
                cout << "Exiting program." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 5);

    return 0;
}
