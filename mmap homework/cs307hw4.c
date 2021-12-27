#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(){
 
	time_t start, end; 
    start = time(NULL); 
    FILE *file;
    file = fopen("loremipsum.txt", "r");
    int c, count=0;
    while ((c = fgetc(file)) != EOF)
    {
    	if((char)c=='a'){
    		count++;
    	}
    }
    end = time(NULL); 
    printf("the number of occurences of 'a' in the file is %d",count);
        printf("\nc fopen time: %d nanoseconds", 
           difftime(end, start)*1000000000); 
    return 0;
}

