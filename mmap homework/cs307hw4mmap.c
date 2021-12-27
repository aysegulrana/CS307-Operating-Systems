#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(){
	time_t start, end; 
    start = time(NULL); 
	int fd=open("loremipsum.txt",O_RDONLY);
	struct stat s;
	size_t size;
	int status=fstat(fd,&s);
	size=s.st_size;
	char *ptr=mmap(0,size,PROT_READ,MAP_PRIVATE,fd,0);
	if(ptr==MAP_FAILED){
		printf("Mapping Failed\n");
		return 1;
	}
	close(fd);
	int count=0;
	size_t i;
	for (i=0;i<=size;i++){
		if(ptr[i]=='a'){
			count++;
		}
	}
	status=munmap(ptr,size);
	end = time(NULL); 
    printf("the number of occurences of 'a' in the file is %d",count);
	printf("\nc mmap time: %d nanoseconds", 
    difftime(end, start)*1000000000); 
	return 0;
}