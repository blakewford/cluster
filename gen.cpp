#include <time.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
    if(argc != 2)
    {
        printf("gen <num points>\n");
        return -1;
    }

    srand(time(NULL));
    long points = strtol(argv[1], NULL, 10);
    while(points--)
    {
        printf("%d\t%d\n", rand()%4000, rand()%6000);
    }
    printf("\n");

    return 0;
}
