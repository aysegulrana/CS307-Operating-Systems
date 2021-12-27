#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <stdlib.h> 
#include <queue> 
#include <semaphore.h>
using namespace std;

#define NUM_THREADS 10
#define MEMORY_SIZE 150

struct node
{
	int id;
	int size;
};

queue<node> myqueue; // shared que
pthread_mutex_t sharedLock = PTHREAD_MUTEX_INITIALIZER; // mutex
pthread_t server; // server thread handle
sem_t semlist[NUM_THREADS]; // thread semaphores

int thread_message[NUM_THREADS]; // thread memory information
char  memory[MEMORY_SIZE]; // memory size

bool cont=true; 


void my_malloc(int thread_id, int size)
{
	//This function will add the struct to the queue it uses mutex to access the shared data
	pthread_mutex_lock(&sharedLock); 
	node insert;
	insert.id=thread_id;
	insert.size=size;
	myqueue.push(insert);
	pthread_mutex_unlock(&sharedLock);
}

void * server_function(void*)
{

	int index=0; //for the next available place
	//This function should grant or decline a thread depending on memory size.
	while(cont) //in order servre thread to continue
	{
		pthread_mutex_lock(&sharedLock); //we will access the shared data 
		if(!myqueue.empty()){
			node x; //we will store the front var of queue here
			x.size=myqueue.front().size; x.id=myqueue.front().id;
			myqueue.pop();
			if(x.size<=(MEMORY_SIZE-index)){	
			//The server thread will store the value in a shared array called thread_message.
				thread_message[x.id]=index; //the starting pt
				cout<<"Starting from "<<index<<". index, "<<x.size<<" bytes are allocated in the memory array."
					<<" (Thread "<<x.id<<")"<<endl;
				index=index+x.size;
			}
			else {
				thread_message[x.id]=-1;
			}
			//requesting thread should be unblocked. 
			sem_post(&semlist[x.id]);
		}
		pthread_mutex_unlock(&sharedLock);
	}	
}

void* thread_function(void* idx) 
{
	//This function will create a random size, and call my_malloc
	int* id= (int*) idx;
	int y=(*id);
	int random=rand()%(MEMORY_SIZE/6)+1;

	my_malloc(y,random);

	//Block	
	sem_wait(&semlist[y]); //wait until server gives a message

	//Then fill the memory with id's or give an error prompt
	pthread_mutex_lock(&sharedLock); //we will access shared data 
	if(thread_message[y]!=-1) {
		//Unblocked thread will check thread_message array and depending on the value it read, it will update the memory
		int start = thread_message[y];
		for(int i=start; i<start+random; i++) {
			memory[i]='0'+y;
		}
	}
	else {
			printf("Thread %d: Not enough memory",y);
	}
	pthread_mutex_unlock(&sharedLock);
	return NULL;
}
void init()	 
{
	pthread_mutex_lock(&sharedLock);	//lock
	for(int i = 0; i < NUM_THREADS; i++) //initialize semaphores
	{sem_init(&semlist[i],0,0);}
	for (int i = 0; i < MEMORY_SIZE; i++)	//initialize memory 
	{char zero = '0'; memory[i] = zero;}
	pthread_create(&server,NULL,server_function,NULL); //start server 
	pthread_mutex_unlock(&sharedLock); //unlock
}

void dump_memory() 
{
	// You need to print the whole memory array here.
	cout<<endl<<"Memory Array Content: "<<endl;
	for (int j=0;j<MEMORY_SIZE;j++){
		cout<<"["<<memory[j]<<"]";
	}
}

int main (int argc, char *argv[])
{

	//You need to create a thread ID array here
	pthread_t threadarr[NUM_THREADS]; 
	int threadid[NUM_THREADS]={0,1,2,3,4,5,6,7,8,9};
	init();	// call init
	//You need to create threads with using thread ID array, using pthread_create()
	for(int i=0; i<NUM_THREADS;i++){
		pthread_create(&threadarr[i],NULL,thread_function,(void*)&threadid[i]);}
	//You need to join the threads
	for(int m=0; m<NUM_THREADS;m++){
		pthread_join(threadarr[m],NULL); }
	cont=false;
	pthread_join(server,NULL);

	for (int k = 0; k < NUM_THREADS; k++) {
		sem_destroy(&semlist[k]);
	}

	dump_memory(); // this will print out the memory

	printf("\nMemory Indexes:\n" );
	for (int i = 0; i < NUM_THREADS; i++)
	{
		printf("[%d]" ,thread_message[i]); // this will print out the memory indexes
	}
	printf("\nTerminating...\n");
}