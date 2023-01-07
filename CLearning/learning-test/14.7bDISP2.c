#include <stdio.h>
#include <string.h>

#define DISP2(f,x,y) printf(#f" 's result is: %s",f(x,y))

int main() {
    char str[40];
    DISP2(strcpy,str,"Good Morning!");
}