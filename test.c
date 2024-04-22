
#include <stdio.h> // printf

int main()
{
    int j = 4;
    
    for (int i = 0; i < 4; i++)
    {
        printf("i = %d\n", i % j);
    }
    return 0;
}