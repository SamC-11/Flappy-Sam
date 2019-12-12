#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    int i;
    
    for(i = 0;i < 4;i++)
    {
        if(i % 2 == 0)
            printf("%d",i);
        else
            printf("\n");      
    }
 

    
    getchar();
    return(0);
}
