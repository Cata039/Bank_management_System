#include "iban_generator.h"
#include <stdlib.h>
#include "constants.h"

// Function to generate IBANs automatically
void generateIBAN(char *iban) {
    const char *validChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    int length = strlen(validChars);

    for (int i = 0; i < 10; i++) {
        iban[i] = validChars[rand() % length];
    }
    iban[10] = '\0';
}
