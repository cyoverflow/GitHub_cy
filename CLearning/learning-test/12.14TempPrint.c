#include <stdio.h>

#define ROW 7
#define COL 24

void readRow(int arr[],int rowNum) {
    int *p, i = 1;
    for(p = arr + COL * (rowNum - 1); p < (arr + COL * rowNum); p++) {
        printf("COL %d is : %d\n",i,*p);
        i++;
    }
}

int main() {
    int temp[ROW][COL], *p;
    int row = 6;
    for (p = &temp[0][0];p < &temp[ROW - 1][COL - 1]; p++) {
        *p = 15;
    }
    temp[5][12] = 32;
    readRow(temp[0],row);
}