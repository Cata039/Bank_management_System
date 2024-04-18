#include "login.h"
#include <stdio.h>
#include <string.h>
#include "account.h"
#include "constants.h"


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
    scanf(" %[^\n]", name);

    // Check if the provided name matches any entry in the file
    for (int i = 0; i < numAccounts; i++) {
        if (strcmp(accounts[i].owner, name) == 0) {
            printf("Login successful!\n");
            return 1; //successful login
        }
    }

    printf("Login failed. Name not found in records.\n");
    return 0;
}
