#include <stdio.h>

enum DAY
{
    MON=1, TUE=3, WED, THU, FRI, SAT, SUN
} day;

int main() {
    // 遍历枚举元素
    for (day = TUE; day <= SUN; day++) {
        printf("枚举元素：%d \n", day);
    }
}