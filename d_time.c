#include "d_time.h"
#include <time.h>

void d_nanosleep(int seconds, int nano_seconds){
    struct timespec sleep_duration;
    sleep_duration.tv_sec = seconds;
    sleep_duration.tv_nsec = nano_seconds;
    nanosleep(&sleep_duration, NULL);
}
