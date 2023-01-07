#include <stdio.h>

#define MUL(x,y) ((x) * (y) < 100 ? 1 : 0)

int main() {
    int n = 10, m = 10;
    printf("%d", MUL(n,m));
    return 0;
}