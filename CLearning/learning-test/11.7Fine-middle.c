#include <stdio.h>

void *find_middle(int [],int);

int main() {
    int arr[] = {12,35,8,99};
    int n = 4, *p;
    p = find_middle(arr, n);
    printf("The middle number is %d.",*p);
}

void *find_middle(int a[], int n) {
    return &a[n / 2];
}