#include <stdio.h>

int main() {
    int a[] = {1,2,3,3,4};
    int *p;
    p = a;
    printf("%d\n",p == &a[0]);
    printf("%d\n",*p == a[0]);
    printf("%d\n",p[0] == a[0]);

}
