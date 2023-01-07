#include <stdio.h>

#define SQR(x) ((x) * (x))


int main() {
#undef SQR

#define SQR
    printf("%d", SQR(5));
}