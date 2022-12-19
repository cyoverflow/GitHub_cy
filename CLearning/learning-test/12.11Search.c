#include <stdio.h>

#define N 10

_Bool search(int a[], int n, int key){
    int *p;
    for(p = a;p < a + n;p++) {
        if(*p == key) {
            return 1;
        }
    }
    return 0;
}

int main() {
    int a[] = {1,2,3,4,5,6,6,7,8,9};
    int key = 99;
    printf("%d", search(a,N,key));
}