#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ACCOUNTS 100

struct Account {
    char name[100];
    char surname[100];
    char iban[100];
    char coin[4];
    double amount;
};

struct Account accounts[MAX_ACCOUNTS];
int numAccounts = 0;

// Function to read account data from CSV file
void readAccountData() {
    FILE *file = fopen("data.csv", "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        exit(1);
    }

    while (fscanf(file, "%s %s %s %lf", accounts[numAccounts].iban, accounts[numAccounts].name,
                  accounts[numAccounts].coin, &accounts[numAccounts].amount) == 4) {
        // Increase the number of accounts read
        numAccounts++;
    }

    fclose(file);
}
// Function to login
void login() {
    char name[100];
    char surname[100];
    scanf("%s %s", name,surname);
}

// Function to create an account
void createAccount() {
    char name[100];
    char surname[100];
    printf("Enter your name: ");
    scanf("%s", name);
    printf("Enter your surname: ");
    scanf("%s", surname);
    printf("Account created successfully!\n");
}

// Function to view account data
void viewAccountData() {
    printf("Viewing account data...\n");
    for (int i = 0; i < numAccounts; i++) {
        printf("Name: %s %s, IBAN: %s, Coin: %s, Amount: %.2lf\n", accounts[i].name, accounts[i].surname,
               accounts[i].iban, accounts[i].coin, accounts[i].amount);
    }
}

// Function to perform transactions
void performTransactions() {
    printf("Performing transactions...\n");
    // Add code to perform transactions here
}

// Function to edit account
void editAccount() {
    printf("Editing account...\n");
    // Add code to edit account here
}

// Function to delete account
void deleteAccount() {
    printf("Deleting account...\n");
    // Add code to delete account here
}

int main() {
    int choice = -1;
    int loggedIn = 0; // Flag to track if user is logged in

    // Read account data from CSV file
    readAccountData();

    printf("Welcome!\n");
    printf("Please press:\n");
    printf("1. LOGIN\n");
    printf("2. CREATE ACCOUNT\n");
    printf("3. EXIT\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    if (choice == 1) {
        printf("Enter your name and surname to login:\n");
        login();
        loggedIn = 1;
    } else if (choice == 2) {
        createAccount();
        loggedIn = 1;
    } else if (choice == 3) {
        printf("Exiting...\n");
        return 0;
    } else {
        printf("Invalid choice! Exiting...\n");
        return 1;
    }

    while (loggedIn) {
        printf("1. VIEW ACCOUNT DATA\n");
        printf("2. PERFORM TRANSACTIONS\n");
        printf("3. EDIT ACCOUNT\n");
        printf("4. DELETE ACCOUNT\n");
        printf("5. LOGOUT\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            viewAccountData();
        } else if (choice == 2) {
            performTransactions();
        } else if (choice == 3) {
            editAccount();
        } else if (choice == 4) {
            deleteAccount();
        } else if (choice == 5) {
            loggedIn = 0;
            printf("Logged out successfully!\n");
        } else {
            printf("Invalid choice! Please enter again.\n");
        }
    }

    printf("Exiting...\n");
    return 0;
}
