#include <stdio.h>

void split_time(long int total_sec, int *hr, int *min, int *sec);

int main() {
    int hour = 0,minute = 0,second = 0;
    long int time_in_sec = 61350;//17:02:30
    split_time(time_in_sec, &hour, &minute, &second);
    printf("Current time is %d : %d : %d",hour,minute,second);
}

void split_time(long int total_sec, int *hr, int *min, int *sec) {
    int hour,minute,second;
    *hr = total_sec / 3600;
    *min = total_sec % 3600 / 60;
    *sec = total_sec % 3600 % 60;
}