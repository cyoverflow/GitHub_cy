#include <stdio.h>

enum DAY
{
    MON=1, TUE=3, WED, THU, FRI, SAT, SUN
} day;

int main() {
    // ����ö��Ԫ��
    for (day = TUE; day <= SUN; day++) {
        printf("ö��Ԫ�أ�%d \n", day);
    }
}