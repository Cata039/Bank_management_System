#ifndef CONSTANTS_H
#define CONSTANTS_H

#define MAX_ACCOUNTS 100
#define MAX_NAME_LENGTH 100
#define MAX_LINE_LENGTH 1000
#define DELIMITER ","
#define TEMP_FILE_NAME "temp.txt"

struct Account {
    char iban[MAX_NAME_LENGTH];
    char owner[MAX_NAME_LENGTH];
    char coin[4];
    double amount;
};

struct CurrencyConversion {
    char currency[4];
    double rate;
};

extern struct Account accounts[MAX_ACCOUNTS];
extern int numAccounts;

extern struct CurrencyConversion conversionRates[];

#endif /* CONSTANTS_H */
