#include "read_data.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "account.h"
#include "constants.h"


// Function to read account data from TXT file
void readAccountData() {
    FILE *file = fopen("persons.txt", "r");

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
