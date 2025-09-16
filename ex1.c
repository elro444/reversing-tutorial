#include <stdio.h>
#include <string.h>

int main() {
    char input[100];
    printf("Enter password: ");
    fgets(input, 100, stdout);

    if (strcmp(input, "apple123") == 0) {
        printf("Access Granted\n");
    } else {
        printf("Wrong password\n");
    }
    return 0;
}
