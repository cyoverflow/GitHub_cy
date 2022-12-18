#include <stdio.h>
#include <string.h>

#define N 100

int main() {
    char msg[N], *p;
    int mLen = 0;

    printf("Enter a message:");
    for(p = msg; p < msg + N; p++) {
        if((*p = getchar()) != '\n') {
            mLen++;
        } else{
            break;
        }
    }

    printf("Reversal is:");
    for(p = msg + mLen - 1; p >= msg; p--) {
        putchar(*p);
    }


}