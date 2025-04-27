#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
#include <limits>

using namespace std;

struct MonthlyBudget {
    map<string, double> categories;
    double total = 0.0;
};

vector<MonthlyBudget> yearlyBudget(12);

void saveToFile() {
    ofstream file("budget_data.txt");
    if (!file) {
        cout << "Error opening file for writing!" << endl;
        return;
    }
    for (int i = 0; i < 12; i++) {
        file << "Month " << i + 1 << "\n";
        for (const auto& category : yearlyBudget[i].categories) {
            file << category.first << ":" << category.second << "\n";
        }
        file << "Total:" << yearlyBudget[i].total << "\n---\n";
    }
    file.close();
}

void loadFromFile() {
    ifstream file("budget_data.txt");
    if (!file) {
        cout << "Error opening file for reading! File will be created when saving data." << endl;
        return;
    }
    yearlyBudget.clear();
    yearlyBudget.resize(12);

    string line;
    int monthIndex = -1;
    while (getline(file, line)) {
        if (line.find("Month") == 0) {
            monthIndex = stoi(line.substr(6)) - 1;
            if (monthIndex < 0 || monthIndex >= 12) {
                cout << "Invalid data in file: " << line << endl;
                continue;
            }
            yearlyBudget[monthIndex].categories.clear();
            yearlyBudget[monthIndex].total = 0.0;
        } else if (line == "---") {
            continue;
        } else {
            size_t colonPos = line.find(':');
            if (colonPos != string::npos) {
                string key = line.substr(0, colonPos);
                string valueStr = line.substr(colonPos + 1);
                double value;
                stringstream(valueStr) >> value;
                if (key == "Total") {
                    yearlyBudget[monthIndex].total = value;
                } else {
                    yearlyBudget[monthIndex].categories[key] = value;
                }
            }
        }
    }
    file.close();
}

void addMonthlyBudget() {
    int month;
    cout << "Enter the month number (1-12): ";
    cin >> month;
    if (cin.fail() || month < 1 || month > 12) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid month number!" << endl;
        return;
    }
    month--; 

    MonthlyBudget& budget = yearlyBudget[month];
    budget.categories.clear();
    budget.total = 0.0;

    int numCategories;
    cout << "Enter the number of categories: ";
    cin >> numCategories;
    cin.ignore(); 

    string category;
    double amount;
    for (int i = 0; i < numCategories; i++) {
        cout << "Enter category name: ";
        getline(cin, category);
        cout << "Enter budget for " << category << ": ";
        cin >> amount;
        cin.ignore(); 

        budget.categories[category] = amount;
        budget.total += amount;
    } 

    saveToFile();
    cout << "Budget for month " << month + 1 << " saved successfully!" << endl;
}

void viewMonthlyBudget() {
    int month;
    cout << "Enter the month number (1-12): ";
    cin >> month;
    if (cin.fail() || month < 1 || month > 12) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid month number!" << endl;
        return;
    }
    month--; 

    const MonthlyBudget& budget = yearlyBudget[month];
    cout << "\nBudget for Month " << month + 1 << ":" << endl;
    for (const auto& category : budget.categories) {
        cout << category.first << ": " << fixed << setprecision(2) << category.second << endl;
    }
    cout << "Total: " << fixed << setprecision(2) << budget.total << endl;
}

void viewYearlyBudget() {
    double yearlyTotal = 0.0;
    cout << "\nYearly Budget Breakdown:" << endl;
    for (int i = 0; i < 12; i++) {
        cout << "Month " << i + 1 << ": Total = " << fixed << setprecision(2) << yearlyBudget[i].total << endl;
        yearlyTotal += yearlyBudget[i].total;
    }
    cout << "Yearly Total: " << fixed << setprecision(2) << yearlyTotal << endl;
}

int main() {
    loadFromFile();
    int choice;
    do {
        cout << "\n1. Add Monthly Budget\n";
        cout << "2. View Monthly Budget\n";
        cout << "3. View Yearly Budget\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

        switch (choice) {
            case 1: addMonthlyBudget(); break;
            case 2: viewMonthlyBudget(); break;
            case 3: viewYearlyBudget(); break;
            default: cout << "Invalid choice, please try again." << endl; break;
        }
    } while (choice != 0);

    return 0;
}
