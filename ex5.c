#include <stdio.h>
#include <string.h>

int check(const char *s) {
    if (strlen(s) != 6) return 0;
    return (s[0] == 'C' && s[1] + s[2] == 200 &&
            s[3] * 2 == 150 && s[4] == 'X' &&
            s[5] - 1 == 'Z');
}

int main() {
    char input[100];
    printf("Enter key: ");
    fgets(input, 100, stdin);
    input[strcspn(input, "\n")] = '\0';

    if (check(input)) {
        printf("Nice!\n");
    } else {
        printf("Nope.\n");
    }
    return 0;
}
