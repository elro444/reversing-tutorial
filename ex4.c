#include <stdio.h>
#include <string.h>

int main() {
    char input[100];
    char secret[] = { 'd' ^ 5, 'o' ^ 5, 'g' ^ 5, 0 }; // "dog" encoded

    printf("Password: ");
    fgets(input, 100, stdin);
    input[strcspn(input, "\n")] = '\0';

    for (int i = 0; input[i]; i++) {
        input[i] ^= 5;
    }

    if (strcmp(input, secret) == 0) {
        printf("You got it!\n");
    } else {
        printf("Wrong.\n");
    }
    return 0;
}
