#include <stdio.h>

void swap(int *p, int *q);

int main() {
    int x = 10, y = 20;
    swap(&x, &y);
    printf("Value of x is %d\nValue of y is %d.",x,y);
    return 0;
}

void swap(int *p, int *q) {
    int temp = 0;
    temp = *p;
    *p = *q;
    *q = temp;
}