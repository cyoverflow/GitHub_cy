#include <stdio.h>

#define REMAINDER(x) ((x) % 4)

int main() {
    int num = 15;
    printf("%d", REMAINDER(num));
    return 0;
}