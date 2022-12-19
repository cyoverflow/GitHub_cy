#include <stdio.h>

#define ROW 7
#define COL 24
void search(int *arr,int key) {
    int *p;
    for(p = &arr[0]; p < (arr + ROW * COL); p++) {
        if(*p == key) {
            printf("Found key!");
            return;
        }
    }
    printf("Key not found!");
}

int main() {
    int temp[ROW][COL], *p;
    int key = 32;
    for (p = &temp[0][0];p < &temp[ROW - 1][COL - 1]; p++) {
        *p = 15;
    }
    temp[5][12] = 32;
    search(temp[0],key);
}