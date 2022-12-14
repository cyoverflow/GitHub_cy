#include <stdio.h>

int main(){
    int c;
    int a = 10;
    c = a++;
    printf("先赋值后运算(a++):\n");
    printf("c的值为:%d\n",c);
    printf("a的值为:%d\n",a);
    a = 10;
    c = a--;
    printf("先赋值后运算(a--):\n");
    printf("c的值为:%d\n",c);
    printf("a的值为:%d\n",a);

    a = 10;
    c = ++a;
    printf("先运算后赋值(++a):\n");
    printf("c的值为:%d\n",c);
    printf("a的值为:%d\n",a);

    a = 10;
    c = --a;
    printf("先运算后赋值(--a):\n");
    printf("c的值为:%d\n",c);
    printf("a的值为:%d\n",a);
    return 0;
}