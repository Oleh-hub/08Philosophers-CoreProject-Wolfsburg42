#include <stdio.h> // printf
#include <sys/time.h> // gettimeofday
#include <unistd.h> // usleep

#include <sys/time.h>	// gettimeofday

static long int time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void ft_usleep(long int time)
{
	long int	start;
    long int    end;

	start = time_ms();
	while (1)
    {
        usleep(100);
        end = time_ms();
        if (end - start >= time)
            break;
    }
}

int main() 
{
    int i = 0;
    struct timeval tv;

    while (1)
    {
        gettimeofday(&tv, NULL);
        printf("Seconds: %ld\n", tv.tv_sec);
        // i = tv.tv_usec / 100000;
        // gettimeofday(&tv, NULL);
        // if ( tv.tv_usec / 100000 > i )
        //     printf("Microseconds: %d\n", tv.tv_usec);
        printf("Microseconds: %d\n", tv.tv_usec);
        printf("ms: %ld\n", tv.tv_sec * 1000 + tv.tv_usec / 1000);
        ft_usleep(200);
        gettimeofday(&tv, NULL);
        printf("Seconds: %ld\n", tv.tv_sec);
        printf("Microseconds: %d\n", tv.tv_usec);
        printf("ms: %ld\n", tv.tv_sec * 1000 + tv.tv_usec / 1000);
        
    }

    return 0;
}
