#include <stdio.h>
#define N 100

void palindrome(int,char*);

int main() {
    //判断是否回文，忽略一切字母以外的字符
    char msg[N], *p;
    int temp;
    int len = 0;

    printf("Enter a message:");
    for (p = msg; p < msg + N; p++) {
        if((temp = getchar()) != '\n') {
            if(temp >= 'a' && temp <= 'z' ){
                *p = (char)temp;
                len++;
            } else if(temp >= 'A' && temp <= 'Z') {
                //统一改成小写
                *p = (char)(temp + 32);
                len++;
            } else {
                //指针左移一格
                p--;
            }
        } else {
            break;
        }
    }

    palindrome(len,msg);

}

void palindrome(int len, char *arr) {
    int i = 1;
    char *p;
    for (p = arr; p < arr + len / 2; p++) {
        if (*p != *(arr + len - i)) {
            printf("Not a palindrome");
            return;
        } else {
            i++;
        }
    }
    printf("Palindrome");
}
