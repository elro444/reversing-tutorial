#include <stdio.h>
#include <string.h>

int main() {
    char input[100];
    char key[] = { 97, 112, 112, 108, 101, 0 }; // "apple"

    printf("Enter password: ");
    fgets(input, 100, stdout);

    if (strcmp(input, key) == 0) {
        printf("Welcome!\n");
    } else {
        printf("Try again.\n");
    }
    return 0;
}
