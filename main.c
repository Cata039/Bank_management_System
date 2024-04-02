#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#define MAX_ACCOUNTS 100
#define MAX_NAME_LENGTH 100
#define MAX_LINE_LENGTH 1000
#define DELIMITER ","

struct Account {
    char iban[100];
    char owner[100];
    char coin[4];
    double amount;
};

struct Account accounts[MAX_ACCOUNTS];
int numAccounts = 0;

// Function to read account data from CSV file
void readAccountData() {
    FILE *file = fopen("C:/Users/jemna/OneDrive/Desktop/PP/C_Bank_Management_system/persons.csv", "r");

    if (file == NULL) {
        printf("Error opening file.\n");
        exit(1);
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, MAX_LINE_LENGTH, file) != NULL && numAccounts < MAX_ACCOUNTS) {
        char *token = strtok(line, DELIMITER); // First token (IBAN)
        strcpy(accounts[numAccounts].iban, token);
        token = strtok(NULL, DELIMITER); // Second token (Owner)
        strcpy(accounts[numAccounts].owner, token);
        token = strtok(NULL, DELIMITER); // Third token (Coin)
        strcpy(accounts[numAccounts].coin, token);
        token = strtok(NULL, DELIMITER); // Fourth token (Amount)
        accounts[numAccounts].amount = atof(token);
        numAccounts++;
    }

    fclose(file);
}

// Function to update CSV file with account data
void updateCSVFile() {
    FILE *file = fopen("persons.csv", "a"); // Open the file in append mode
    if (file == NULL) {
        printf("Error opening file.\n");
        exit(1);
    }

    // Write the new account information at the end of the file
    fprintf(file, "%s,%s,%s,%.2lf\n", accounts[numAccounts - 1].iban, accounts[numAccounts - 1].owner,
            accounts[numAccounts - 1].coin, accounts[numAccounts - 1].amount);

    fclose(file);
}

// Function to find accounts owned by a person
int findAccountsByOwner(const char *name, int indexes[]) {
    int count = 0;
    for (int i = 0; i < numAccounts; i++) {
        if (strcmp(accounts[i].owner, name) == 0) {
            indexes[count++] = i;
        }
    }
    return count;
}

// Function to login
int login(char *name) {
    printf("Enter your name and surname: ");
    scanf(" %[^\n]", name); // Read full name with spaces

    // Convert the entered name to lowercase for case-insensitive comparison
    char lowercaseName[MAX_NAME_LENGTH];
    strcpy(lowercaseName, name);
    for (int i = 0; lowercaseName[i]; i++) {
        lowercaseName[i] = tolower(lowercaseName[i]);
    }

    // Check if the provided name matches any entry in the data.csv file
    for (int i = 0; i < numAccounts; i++) {
        char lowercaseOwner[MAX_NAME_LENGTH];
        strcpy(lowercaseOwner, accounts[i].owner);
        for (int j = 0; lowercaseOwner[j]; j++) {
            lowercaseOwner[j] = tolower(lowercaseOwner[j]);
        }
        if (strcmp(lowercaseOwner, lowercaseName) == 0) {
            printf("Login successful!\n");
            return 1; //successful login
        }
    }

    printf("Login failed. Name not found in records.\n");
    return 0;
}

// Function to create an account
void createAccount() {
    printf("Enter the IBAN: ");
    scanf("%s", accounts[numAccounts].iban);

    printf("Enter the owner's name: ");
    getchar(); // Consume the newline character left in the buffer
    fgets(accounts[numAccounts].owner, sizeof(accounts[numAccounts].owner), stdin);
    accounts[numAccounts].owner[strcspn(accounts[numAccounts].owner, "\n")] = '\0'; // Remove trailing newline

    printf("Enter the coin type (e.g., EUR, USD, RON): ");
    scanf("%s", accounts[numAccounts].coin);

    printf("Enter the amount: ");
    scanf("%lf", &accounts[numAccounts].amount);

    numAccounts++;
    printf("Account created successfully!\n");

    // Update the CSV file with the new account data
    updateCSVFile();
}

// Function to edit account
void editAccount(const char *name) {
    int indexes[MAX_ACCOUNTS];
    int count = findAccountsByOwner(name, indexes);

    if (count == 0) {
        printf("You don't own any accounts.\n");
        return;
    }

    printf("Select an account to edit:\n");
    for (int i = 0; i < count; i++) {
        printf("%d. IBAN: %s, Coin: %s, Amount: %.2lf\n", i + 1, accounts[indexes[i]].iban,
               accounts[indexes[i]].coin, accounts[indexes[i]].amount);
    }

    int choice;
    printf("Enter your choice: ");
    scanf("%d", &choice);

    if (choice < 1 || choice > count) {
        printf("Invalid choice.\n");
        return;
    }

    int selectedIndex = indexes[choice - 1];
    printf("Enter new IBAN: ");
    scanf("%s", accounts[selectedIndex].iban);
    printf("Enter new amount: ");
    scanf("%lf", &accounts[selectedIndex].amount);
    printf("Enter new coin type (e.g., EUR, USD, RON): ");
    scanf("%s", accounts[selectedIndex].coin);

    printf("Account edited successfully!\n");
    updateCSVFile();
}

// Function to delete account
void deleteAccount(const char *name) {
    int indexes[MAX_ACCOUNTS];
    int count = findAccountsByOwner(name, indexes);

    if (count == 0) {
        printf("You don't own any accounts.\n");
        return;
    }

    printf("Select an account to delete:\n");
    for (int i = 0; i < count; i++) {
        printf("%d. IBAN: %s, Coin: %s, Amount: %.2lf\n", i + 1, accounts[indexes[i]].iban,
               accounts[indexes[i]].coin, accounts[indexes[i]].amount);
    }

    int choice;
    printf("Enter your choice: ");
    scanf("%d", &choice);

    if (choice < 1 || choice > count) {
        printf("Invalid choice.\n");
        return;
    }

    int selectedIndex = indexes[choice - 1];
    for (int i = selectedIndex; i < numAccounts - 1; i++) {
        accounts[i] = accounts[i + 1];
    }
    numAccounts--;

    printf("Account deleted successfully!\n");
    updateCSVFile();
}

// Function to view account data
void viewAccountData(const char *name) {
    int indexes[MAX_ACCOUNTS];
    int count = findAccountsByOwner(name, indexes);

    if (count == 0) {
        printf("You don't own any accounts.\n");
        return;
    }

    printf("Your accounts:\n");
    for (int i = 0; i < count; i++) {
        printf("IBAN: %s, Coin: %s, Amount: %.2lf\n", accounts[indexes[i]].iban,
               accounts[indexes[i]].coin, accounts[indexes[i]].amount);
    }
}

// Function to perform transactions
void performTransactions(const char *name) {
    char sourceIban[100], destIban[100];
    double amount;

    printf("Enter source IBAN: ");
    scanf("%s", sourceIban);
    printf("Enter destination IBAN: ");
    scanf("%s", destIban);
    printf("Enter amount to transfer: ");
    scanf("%lf", &amount);

    // Check if source account belongs to the user
    int sourceIndex = -1;
    for (int i = 0; i < numAccounts; i++) {
        if (strcmp(accounts[i].iban, sourceIban) == 0 && strcmp(accounts[i].owner, name) == 0) {
            sourceIndex = i;
            break;
        }
    }

    if (sourceIndex == -1) {
        printf("You don't own the source account or it does not exist.\n");
        return;
    }

    // Check if destination account exists
    int destIndex = -1;
    for (int i = 0; i < numAccounts; i++) {
        if (strcmp(accounts[i].iban, destIban) == 0) {
            destIndex = i;
            break;
        }
    }

    if (destIndex == -1) {
        printf("Destination account does not exist.\n");
        return;
    }

    // Perform transaction
    if (accounts[sourceIndex].amount >= amount) {
        accounts[sourceIndex].amount -= amount;
        accounts[destIndex].amount += amount;
        printf("Transaction successful!\n");
        updateCSVFile();
    } else {
        printf("Insufficient funds in the source account.\n");
    }
}
int main() {
    int choice = -1;
    char name[100];
    int loggedIn = 0; // Flag to track if user is logged in
    int accountCreated = 0; // Flag to track if account has been created

    // Read account data from CSV file
    readAccountData();

    printf("Welcome!\n");

    while (1) {
        if (!loggedIn) {
            printf("Please press:\n");
            printf("1. LOGIN\n");
            printf("2. CREATE ACCOUNT\n");
            printf("3. EXIT\n");

            printf("Enter your choice: ");
            scanf("%d", &choice);

            switch (choice) {
                case 1:
                    loggedIn = login(name);
                    break;
                case 2:
                    createAccount();
                    accountCreated = 1;
                    break;
                case 3:
                    printf("Exiting...\n");
                    return 0;
                default:
                    printf("Invalid choice! Please enter again.\n");
                    break;
            }
        } else {
            printf("\nMenu:\n");
            printf("1. VIEW ACCOUNT DATA\n");
            printf("2. PERFORM TRANSACTIONS\n");
            printf("3. EDIT ACCOUNT\n");
            printf("4. DELETE ACCOUNT\n");
            printf("5. LOGOUT\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);

            switch (choice) {
                case 1:
                    viewAccountData(name);
                    break;
                case 2:
                    performTransactions(name);
                    break;
                case 3:
                    editAccount(name);
                    break;
                case 4:
                    deleteAccount(name);
                    break;
                case 5:
                    loggedIn = 0;
                    printf("Logged out successfully!\n");
                    break;
                default:
                    printf("Invalid choice! Please enter again.\n");
                    break;
            }
        }
    }

    printf("Exiting...\n");
    return 0;
}
