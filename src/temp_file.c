#include "temp_file.h"
#include <stdio.h>
#include "constants.h"


void updateTXTFile() {
    FILE *file = fopen("persons.txt", "a");
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

// Function to merge temporary file with main TXT file
void mergeTempFileWithTXT() {
    FILE *mainFile = fopen("persons.txt", "w");
    FILE *tempFile = fopen(TEMP_FILE_NAME, "r");

    if (mainFile == NULL || tempFile == NULL) {
        printf("Error opening files for merging.\n");
        exit(1);
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, MAX_LINE_LENGTH, tempFile) != NULL) {
        fputs(line, mainFile); // Append each line from the temp file to the main TXT file
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
