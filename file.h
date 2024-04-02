#ifndef FILE_H
#define FILE_H

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

void readAccountData();
void updateCSVFile();
int findAccountsByOwner(const char *name, int indexes[]);
int login(char *name);
void createAccount();
void editAccount(const char *name);
void deleteAccount(const char *name);
void viewAccountData(const char *name);
void performTransactions(const char *name);

#endif /* FILE_H */
