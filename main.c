#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ACCOUNTS 100

struct Account {
    char iban[100];
    char ownerName[100];
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

    while (fscanf(file, "%s %s %s %lf", accounts[numAccounts].iban, accounts[numAccounts].ownerName,
                  accounts[numAccounts].coin, &accounts[numAccounts].amount) == 4) {
        // Increase the number of accounts read
        numAccounts++;
    }

    fclose(file);
}
// Function to login
void login() {
    char name[100];
    scanf("%s %s", name);
}

// Function to update CSV file with account data
void updateCSVFile() {
    FILE *file = fopen("data.csv", "w");
    if (file == NULL) {
        printf("Error opening file.\n");
        exit(1);
    }

    for (int i = 0; i < numAccounts; i++) {
        fprintf(file, "%s %s %s %.2lf\n", accounts[i].iban, accounts[i].ownerName, accounts[i].coin, accounts[i].amount);
    }

    fclose(file);
}

// Function to create an account
void createAccount() {
    printf("Enter the IBAN: ");
    scanf("%s", accounts[numAccounts].iban);
    printf("Enter the owner's name: ");
    scanf("%s", accounts[numAccounts].ownerName);
    printf("Enter the coin type (e.g., EUR, USD, RON): ");
    scanf("%s", accounts[numAccounts].coin);
    printf("Enter the amount: ");
    scanf("%lf", &accounts[numAccounts].amount);

    numAccounts++;
    printf("Account created successfully!\n");

    // Update the CSV file with the new account data
    updateCSVFile();
}

// Function to view account data
void viewAccountData() {
    printf("Viewing account data...\n");
    for (int i = 0; i < numAccounts; i++) {
        printf("IBAN: %s, Owner's Name: %s, Coin: %s, Amount: %.2lf\n", accounts[i].iban, accounts[i].ownerName,
               accounts[i].coin, accounts[i].amount);
    }
}

// Function to perform transactions
void performTransactions() {
    char sourceIban[100], destIban[100];
    double amount;
    printf("Enter source IBAN: ");
    scanf("%s", sourceIban);
    printf("Enter destination IBAN: ");
    scanf("%s", destIban);
    printf("Enter amount to transfer: ");
    scanf("%lf", &amount);

    // Search for source and destination accounts
    int sourceIndex = -1, destIndex = -1;
    for (int i = 0; i < numAccounts; i++) {
        if (strcmp(accounts[i].iban, sourceIban) == 0) {
            sourceIndex = i;
        }
        if (strcmp(accounts[i].iban, destIban) == 0) {
            destIndex = i;
        }
    }

    if (sourceIndex != -1 && destIndex != -1) {
        if (accounts[sourceIndex].amount >= amount) {
            accounts[sourceIndex].amount -= amount;
            accounts[destIndex].amount += amount;
            printf("Transaction successful!\n");
            // Update the CSV file with the updated account data
            updateCSVFile();
        } else {
            printf("Insufficient funds!\n");
        }
    } else {
        printf("Source or destination account not found!\n");
    }
}

// Function to edit account
void editAccount() {
    char iban[100];
    printf("Enter the IBAN of the account to edit: ");
    scanf("%s", iban);

    int found = 0;
    for (int i = 0; i < numAccounts; i++) {
        if (strcmp(accounts[i].iban, iban) == 0) {
            printf("Enter the new amount: ");
            scanf("%lf", &accounts[i].amount);
            printf("Enter the new coin type (e.g., EUR, USD, RON): ");
            scanf("%s", accounts[i].coin);
            found = 1;
            break;
        }
    }

    if (found) {
        printf("Account details updated successfully!\n");
        // Update the CSV file with the updated account data
        updateCSVFile();
    } else {
        printf("Account not found!\n");
    }
}

// Function to delete account
void deleteAccount() {
    char deleteIban[100];
    printf("Enter the IBAN of the account to delete: ");
    scanf("%s", deleteIban);

    for (int i = 0; i < numAccounts; i++) {
        if (strcmp(accounts[i].iban, deleteIban) == 0) {
            // Shift accounts to fill the gap
            for (int j = i; j < numAccounts - 1; j++) {
                accounts[j] = accounts[j + 1];
            }
            numAccounts--;
            printf("Account deleted successfully!\n");

            // Update CSV file
            updateCSVFile();
            return;
        }
    }

    printf("Account with IBAN %s not found.\n", deleteIban);
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
