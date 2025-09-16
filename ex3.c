#include <stdio.h>

int main() {
    int num;
    printf("Enter the secret number: ");
    scanf("%d", &num);

    if (num * 3 + 7 == 100) {
        printf("Correct!\n");
    } else {
        printf("Nope.\n");
    }
    return 0;
}
