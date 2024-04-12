
#include <stdio.h> // printf
#include <unistd.h> // fork
#include <time.h>   // clock_gettime, nanosleep

void timewait(int milliseconds)
{
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000;
    nanosleep(&ts, NULL);
}

int main() {
    int i;
    struct timespec start, end;
    
    for (i = 0; i < 3; i++) {
        fork();
        clock_gettime(CLOCK_REALTIME, &start);
        timewait(500); // sleep for 500 milliseconds
        clock_gettime(CLOCK_REALTIME, &end);
        printf("Time elapsed: %ld nanoseconds\n", (end.tv_sec - start.tv_sec)
             * 1000000000 + (end.tv_nsec - start.tv_nsec));
    }
    return 0;
}