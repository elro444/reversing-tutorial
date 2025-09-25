#include <stdio.h>
#include <string.h>

int main() {
    char input[100];
    int sum = 0;

    printf("Enter the code: ");
    fgets(input, 100, stdin);
    input[strcspn(input, "\n")] = '\0';

    for (int i = 0; i < strlen(input); i++) {
        sum += ((unsigned char)input[i]) * (i + 1);
        printf("Got sum=%d\n", sum);
    }

    if (sum == 1337) {
        printf("Correct code!\n");
    } else {
        printf("Wrong.\n");
    }
    return 0;
}
