#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <ctime>
#include <chrono>

using namespace std;

int main(){
	std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
	std::chrono::time_point<std::chrono::high_resolution_clock> endTime;
    startTime=std::chrono::high_resolution_clock::now();
	ifstream infile("loremipsum.txt");
	char c;
	int count=0;

	while (infile.get(c)) {   // loop getting single characters
		if(c=='a'){
			count++;
		}
	}
	infile.close();        // close file
	endTime=std::chrono::high_resolution_clock::now();
	auto timex=(std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count());
	cout<<"the number of occurences of 'a' in the file is "<<count<<endl;
	cout<<"cpp fstream time: "<<timex<<" nanoseconds"<<endl;
	return 0;
}