#include <stdio.h>

#define CUBE(x) ((x)*(x)*(x))

int main() {
    int num = 4 + 1;
    printf("%d",CUBE(num));
    return 0;
}