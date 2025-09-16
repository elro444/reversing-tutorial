#include <stdio.h>
#include <string.h>

int main() {
    char input[100];
    int sum = 0;

    printf("Enter the code: ");
    fgets(input, 100, stdout);

    for (int i = 0; i < strlen(input); i++) {
        sum += input[i] * (i + 1);
    }

    if (sum == 1337) {
        printf("Correct code!\n");
    } else {
        printf("Wrong.\n");
    }
    return 0;
}
