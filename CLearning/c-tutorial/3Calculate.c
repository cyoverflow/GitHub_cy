#include <stdio.h>

int main(){
    int c;
    int a = 10;
    c = a++;
    printf("�ȸ�ֵ������(a++):\n");
    printf("c��ֵΪ:%d\n",c);
    printf("a��ֵΪ:%d\n",a);
    a = 10;
    c = a--;
    printf("�ȸ�ֵ������(a--):\n");
    printf("c��ֵΪ:%d\n",c);
    printf("a��ֵΪ:%d\n",a);

    a = 10;
    c = ++a;
    printf("�������ֵ(++a):\n");
    printf("c��ֵΪ:%d\n",c);
    printf("a��ֵΪ:%d\n",a);

    a = 10;
    c = --a;
    printf("�������ֵ(--a):\n");
    printf("c��ֵΪ:%d\n",c);
    printf("a��ֵΪ:%d\n",a);
    return 0;
}