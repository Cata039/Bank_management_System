#include "transactions.h"
#include <stdio.h>
#include <string.h>
#include "account.h"
#include "constants.h"


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
            printf("Transaction failed: Currency conversion not supported.\n");
            return;
        }

        if (accounts[sourceIndex].amount >= amount) {
            accounts[sourceIndex].amount -= amount;
            accounts[destIndex].amount += convertedAmount;
            printf("Transaction successful! Converted amount: %.2lf %s\n", convertedAmount, accounts[destIndex].coin);
            updateTXTFile();
        } else {
            printf("Insufficient funds in the source account.\n");
        }
    } else {
        // No currency conversion needed
        if (accounts[sourceIndex].amount >= amount) {
            accounts[sourceIndex].amount -= amount;
            accounts[destIndex].amount += amount;
            printf("Transaction successful!\n");
            updateTXTFile();
        } else {
            printf("Insufficient funds in the source account.\n");
        }
    }

    createTempFile();
    mergeTempFileWithTXT();
    clearTempFile();
}
