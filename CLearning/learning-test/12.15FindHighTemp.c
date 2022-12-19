#include <stdio.h>

#define ROW 7
#define COL 24

int findLargest(int[]);
void printHighestTemp(int[]);
void printArr(int*);//打印数组

int main() {
    int temp[ROW][COL], *p;
    for (p = temp[0];p < &temp[0][0] + ROW * COL; p++) {
        *p = 15;
    }
    for(int i = 0,j = 1; i < ROW; i++,j++) {
        temp[i][j] = 18 + j;
    }
    //printArr(temp[0]);
    printHighestTemp(temp[0]);
}

void printHighestTemp(int arr[]) {
    int *p;
    int i = 1;
    for(p = arr; p < arr + ROW * COL;p += COL) {
        printf("Highest temperature of day %d is : %d\n",i, findLargest(p));
        i++;
    }
}

int findLargest(int *ptr) {
    int *p, max = *ptr;
    for(p = ptr + 1; p < ptr + COL; p++) {
        //printf("%d ",*p);
        if(max < *p) {
            max = *p;
        }
    }
    return max;
}

void printArr(int *arr) {
    int i = 1, *p;
    for(p = arr;p < arr + ROW * COL; p++) {
        if(i % 24 == 1) {
            printf("Row %d:",i / 24 + 1);
            printf("%d ",*p);
        } else if(i % 24 == 0){
            printf("%d ",*p);
            printf("\n");
        } else {
            printf("%d ",*p);
        }
        i++;
    }
}