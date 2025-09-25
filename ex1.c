#include <stdio.h>
#include <string.h>

int main() {
    char input[100];

    printf("Enter password: ");
    fgets(input, 100, stdin);
    input[strcspn(input, "\n")] = '\0';

    if (strcmp(input, "apple123") == 0) {
        printf("Access Granted\n");
    } else {
        printf("Wrong password\n");
    }
    return 0;
}
