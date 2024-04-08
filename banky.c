#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#define MAX_ACCOUNTS 100
#define MAX_NAME_LENGTH 100
#define MAX_LINE_LENGTH 1000
#define DELIMITER ","
#define TEMP_FILE_NAME "temp.csv"

struct Account {
    char iban[100];
    char owner[100];
    char coin[4];
    double amount;
};

// Define a structure for storing currency conversion rates
struct CurrencyConversion {
    char currency[4];
    double rate;
};

// Sample conversion rates (you can extend this or fetch from an external source)
struct CurrencyConversion conversionRates[] = {
        {"USD", 1.18}, // 1 EUR = 1.18 USD
        {"RON", 4.91}  // 1 EUR = 4.91 RON
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
    FILE *file = fopen("C:/Users/jemna/OneDrive/Desktop/PP/C_Bank_Management_system/persons.csv", "a");
    if (file == NULL) {
        printf("Error opening file.\n");
        exit(1);
    }

    // Write the new account information at the end of the file
    fprintf(file, "%s,%s,%s,%.2lf\n", accounts[numAccounts - 1].iban, accounts[numAccounts - 1].owner,
            accounts[numAccounts - 1].coin, accounts[numAccounts - 1].amount);

    fclose(file);
}

// Function to create a temporary file with changing data
void createTempFile() {
    FILE *file = fopen(TEMP_FILE_NAME, "w");
    if (file == NULL) {
        printf("Error creating temporary file.\n");
        exit(1);
    }

    // Write the changing account information to the temporary file
    for (int i = 0; i < numAccounts; i++) {
        fprintf(file, "%s,%s,%s,%.2lf\n", accounts[i].iban, accounts[i].owner,
                accounts[i].coin, accounts[i].amount);
    }

    fclose(file);
}

// Function to merge temporary file with main CSV file
void mergeTempFileWithCSV() {
    FILE *mainFile = fopen("C:/Users/jemna/OneDrive/Desktop/PP/C_Bank_Management_system/persons.csv", "w");
    FILE *tempFile = fopen(TEMP_FILE_NAME, "r");

    if (mainFile == NULL || tempFile == NULL) {
        printf("Error opening files for merging.\n");
        exit(1);
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, MAX_LINE_LENGTH, tempFile) != NULL) {
        fputs(line, mainFile); // Append each line from the temp file to the main CSV file
    }

    fclose(mainFile);
    fclose(tempFile);
}


// Function to clear temporary file
void clearTempFile() {
    FILE *file = fopen(TEMP_FILE_NAME, "w");
    if (file == NULL) {
        printf("Error clearing temporary file.\n");
        exit(1);
    }
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

    // Check if the provided name matches any entry in the persons.csv file
    for (int i = 0; i < numAccounts; i++) {
        if (strcmp(accounts[i].owner, name) == 0) {
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
    getchar(); // Consume newline character left in the buffer

    printf("Enter the owner's name: ");
    fgets(accounts[numAccounts].owner, sizeof(accounts[numAccounts].owner), stdin);
    accounts[numAccounts].owner[strcspn(accounts[numAccounts].owner, "\n")] = '\0'; // Remove trailing newline

    printf("Enter the coin type (e.g., EUR, USD, RON): ");
    scanf("%s", accounts[numAccounts].coin);
    getchar(); // Consume newline character left in the buffer

    printf("Enter the amount: ");
    scanf("%lf", &accounts[numAccounts].amount);

    numAccounts++;
    printf("Account created successfully!\n");

    createTempFile();
    updateCSVFile();
    clearTempFile();
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
    int editChoice;

    printf("Select what to edit:\n");
    printf("1. IBAN\n");
    printf("2. Amount\n");
    printf("3. Coin\n");
    printf("Enter your choice: ");
    scanf("%d", &editChoice);

    switch (editChoice) {
        case 1:
            printf("Enter new IBAN: ");
            scanf("%s", accounts[selectedIndex].iban);
            break;
        case 2:
            printf("Enter new amount: ");
            scanf("%lf", &accounts[selectedIndex].amount);
            break;
        case 3:
            printf("Enter new coin type (e.g., EUR, USD, RON): ");
            scanf("%s", accounts[selectedIndex].coin);
            break;
        default:
            printf("Invalid choice.\n");
            return;
    }

    printf("Account edited successfully!\n");
    createTempFile();
    mergeTempFileWithCSV();
    clearTempFile();
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

    // Remove the account from the array
    for (int i = selectedIndex; i < numAccounts - 1; i++) {
        accounts[i] = accounts[i + 1];
    }
    numAccounts--;

    // Rewrite the CSV file with updated account data
    FILE *file = fopen("persons.csv", "w");
    if (file == NULL) {
        printf("Error opening file.\n");
        exit(1);
    }

    for (int i = 0; i < numAccounts; i++) {
        fprintf(file, "%s,%s,%s,%.2lf\n", accounts[i].iban, accounts[i].owner,
                accounts[i].coin, accounts[i].amount);
    }

    fclose(file);

    printf("Account deleted successfully!\n");

    createTempFile();
    mergeTempFileWithCSV();
    clearTempFile();
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


// Function to convert currency
double convertCurrency(double amount, const char *sourceCurrency, const char *destCurrency) {
    double conversionRate;
    if (strcmp(sourceCurrency, "RON") == 0 && strcmp(destCurrency, "EUR") == 0) {
        conversionRate = 0.20; // 1 RON = 0.20 EUR
    } else if (strcmp(sourceCurrency, "RON") == 0 && strcmp(destCurrency, "USD") == 0) {
        conversionRate = 0.22; // 1 RON = 0.22 USD
    } else if (strcmp(sourceCurrency, "EUR") == 0 && strcmp(destCurrency, "RON") == 0) {
        conversionRate = 5.0; // 1 EUR = 5.0 RON
    } else if (strcmp(sourceCurrency, "USD") == 0 && strcmp(destCurrency, "RON") == 0) {
        conversionRate = 4.55; // 1 USD = 4.55 RON
    } else {
        printf("Conversion not supported between %s and %s.\n", sourceCurrency, destCurrency);
        return -1.0; // Return -1 to indicate conversion not supported
    }

    return amount * conversionRate;
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

    // Perform transaction with currency conversion if necessary
    if (strcmp(accounts[sourceIndex].coin, accounts[destIndex].coin) != 0) {
        // Currency conversion is needed
        double convertedAmount = convertCurrency(amount, accounts[sourceIndex].coin, accounts[destIndex].coin);
        if (convertedAmount < 0) {
            // Conversion not supported
            printf("Transaction failed: Currency conversion not supported.\n");
            return;
        }

        if (accounts[sourceIndex].amount >= amount) {
            accounts[sourceIndex].amount -= amount;
            accounts[destIndex].amount += convertedAmount;
            printf("Transaction successful! Converted amount: %.2lf %s\n", convertedAmount, accounts[destIndex].coin);
            updateCSVFile();
        } else {
            printf("Insufficient funds in the source account.\n");
        }
    } else {
        // No currency conversion needed
        if (accounts[sourceIndex].amount >= amount) {
            accounts[sourceIndex].amount -= amount;
            accounts[destIndex].amount += amount;
            printf("Transaction successful!\n");
            updateCSVFile();
        } else {
            printf("Insufficient funds in the source account.\n");
        }
    }

    createTempFile();
    mergeTempFileWithCSV();
    clearTempFile();
}




int main() {
    int choice = -1;
    char name[100];
    int loggedIn = 0; // Flag to track if user is logged in
    int accountCreated = 0; // Flag to track if account has been created

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