#include <stdio.h>

void find_two_largest(int [],int,int *,int *);

int main() {
    int arr[] = {12,35,8,99,326,59};
    int n = 6, largest = 0, second = 0;
    find_two_largest(arr, n, &largest, &second);
    printf("The largest number is %d.\nSecond is %d.",largest,second);
}

void find_two_largest(int a[], int n, int *largest, int *second_largest) {
    if(a[0] > a[1]) {
        *largest = a[0];
        *second_largest = a[1];
    } else {
        *largest = a[1];
        *second_largest = a[0];
    }

    for (int i = 2; i < n; i++) {
        if(a[i] > *largest){
            *second_largest = *largest;
            *largest = a[i];
        } else if(a[i] > *second_largest) {
            *second_largest = a[i];
        }
    }
}