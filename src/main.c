#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "iban_generator.h"
#include "transactions.h"
#include "account.h"
#include "login.h"
#include "temp_file.h"
#include "read_data.h"
#include "constants.h"



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
