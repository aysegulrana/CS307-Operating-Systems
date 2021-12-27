#include <iostream>
#include <string>
#include <time.h> 
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
using namespace std;

int plane[2][50]={0}; //global matrix
int turn=0; //busy waiting variable
int remaining=100; //checks the remaining number of seats
bool exit1=false, exit2=false; //if threads are terminated, we make them true

void* TravelAgency1 (void*){
	while (remaining!=0){
		int row, col;
		int random=rand() % 100 + 1; //generate random number between 1 and 100
		if(random>50) { //indicating its place on the plane, finding row and column
			row=1; 
			col=random-51;}
		else {
			row=0;
			col=random-1;
		}
		while(turn!=0){} //busy waiting
		printf(" \nAgency 1 Entered Critical Region \n");
		if (remaining>0){
				if(plane[row][col]==0){ //if the seat is available
					plane[row][col]=1; remaining--; //book it and reduce 1
					printf("Seat Number %d", random); 
					printf(" is reserved by Agency 1 \n");
					if (remaining==0) {exit1=true;} //if this was the last seat, this is our last while loop.
				}		
		}		
		else {exit1=true;} //if the other thred booked the last one, this is our last while loop
		printf("Agency 1 Exit Critical Region \n");
		turn=1; 	//let other thread go out of the busy waiting loop		
	}
}

void* TravelAgency2 (void*){
	while (remaining!=0){
		int row, col;
		int random=rand() % 100 + 1;
		if(random>50) {
			row=1; 
			col=random-51;}
		else {
			row=0;
			col=random-1;
		}

		while(turn!=1){}

		printf(" \nAgency 2 Entered Critical Region \n");
		if (remaining>0){		
				if(plane[row][col]==0){
					plane[row][col]=2; remaining--;
					printf("Seat Number %d", random); 
					printf(" is reserved by Agency 2 \n");
					if (remaining==0) {exit2=true;}
				}		
		}	
		else{exit2=true;}
		printf("Agency 2 Exit Critical Region \n");	
		turn=0; 
	}
}

int main(){
	srand (time(NULL));
	pthread_t thread1, thread2;
	pthread_create(&thread1,NULL,TravelAgency1,NULL);
	pthread_create(&thread2, NULL, TravelAgency2, NULL);

	while(remaining!=0 && (!exit1 || !exit2)){ } 
	//wait until the remaining=0 and both threads exit the functions 
	cout<<" \nNo Seats Left";
	cout<<endl<<" \nPlane is full: "<<endl;
	//we print out the plane matrix
	for (int i=0; i<2; i++){
		for (int j=0;j<50; j++){
			cout<<plane[i][j]<<" "; 
		} 
		cout<<endl;
	}
	pthread_join(thread1,NULL);
	pthread_join(thread2,NULL);
	return 0;
}