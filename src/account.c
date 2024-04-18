#include "account.h"
#include <stdio.h>
#include <string.h>
#include "constants.h"

struct Account accounts[MAX_ACCOUNTS];
int numAccounts = 0;

// Function to create an account
void createAccount() {
    generateIBAN(accounts[numAccounts].iban);

    getchar(); // Consume newline character left in the buffer

    printf("Enter the owner's name: ");
    fgets(accounts[numAccounts].owner, sizeof(accounts[numAccounts].owner), stdin);
    accounts[numAccounts].owner[strcspn(accounts[numAccounts].owner, "\n")] = '\0'; // Remove trailing newline

    // Select coin type
    int coinChoice;
    printf("Select the coin type:\n");
    printf("1. RON\n");
    printf("2. EUR\n");
    printf("3. USD\n");
    printf("Enter your choice: ");
    scanf("%d", &coinChoice);
    getchar(); // Consume the newline character left in the input buffer after scanf

    switch (coinChoice) {
        case 1:
            strcpy(accounts[numAccounts].coin, "RON");
            break;
        case 2:
            strcpy(accounts[numAccounts].coin, "EUR");
            break;
        case 3:
            strcpy(accounts[numAccounts].coin, "USD");
            break;
        default:
            printf("Invalid choice! Setting coin type to RON by default.\n");
            strcpy(accounts[numAccounts].coin, "RON");
            break;
    }

    printf("Enter the amount: ");
    scanf("%lf", &accounts[numAccounts].amount);

    printf("Account created successfully!\n");

    numAccounts++;

    createTempFile();
    updateTXTFile();
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
    mergeTempFileWithTXT();
    clearTempFile();
}


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

    // Open the file for writing
    FILE *file = fopen("persons.txt", "w");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        exit(1);
    }

    // Remove the account from the array
    for (int i = selectedIndex; i < numAccounts - 1; i++) {
        accounts[i] = accounts[i + 1];
    }
    numAccounts--;

    // Write the updated account data to the file
    for (int i = 0; i < numAccounts; i++) {
        fprintf(file, "%s,%s,%s,%.2lf\n", accounts[i].iban, accounts[i].owner,
                accounts[i].coin, accounts[i].amount);
    }

    // Close the file
    fclose(file);

    printf("Account deleted successfully!\n");

    createTempFile();
    mergeTempFileWithTXT();
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
