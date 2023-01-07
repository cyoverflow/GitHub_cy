#include <stdio.h>

#define NELEMS(a) ((int) (sizeof(a) / sizeof(a[0])))

int main() {
    int arr[] = {1,2,3,4,5,6,7,8};
    printf("Number of array is: %d", NELEMS(arr));
    return 0;
}