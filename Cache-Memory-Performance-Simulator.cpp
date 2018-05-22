#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <iostream>
#include <fstream>

using namespace std;

struct CacheLine{
	unsigned long int TAG;
	unsigned char checker=0; 
};

void DirectMappingByColumn(int &hits, int &miss, unsigned char memory[100][1000], CacheLine cache[64]){
	unsigned long int addr;
	unsigned long int tag;
	unsigned int line;

	//Accessing the cache by its column
	for(int i=0; i<100; i++){
		for(int j=0; j<1000; j++){
			addr = (unsigned long int)(&memory[i][j]);
			line = ((addr & 0b1111110000)>>4); //taking the line
			tag = (addr >> 10); //taking the tag

			if(cache[line].checker == 0){ //if position is empty
				miss++;
				cache[line].checker = 1;
				cache[line].TAG = tag;
			}
			else{
				if(cache[line].TAG == tag){ //if position isn't empty and line tag equal cache tag
					hits++;
				}
				else{
					miss++;
					cache[line].TAG = tag;
				}
			}

		}
	}
}

void DirectMappingByLine(int &hits, int &miss, unsigned char memory[100][1000], CacheLine cache[64]){
	unsigned long int addr;
	unsigned long int tag;
	unsigned int line;

	//Accessing the cache by its lines
	for(int j=0; j<1000; j++){
		for(int i=0; i<100; i++){
			addr = (unsigned long int)(&memory[i][j]);
			line = ((addr & 0b1111110000)>>4); //taking the line
			tag = (addr >> 10); //taking the address

			if(cache[line].checker == 0){ //if position is empty
				miss++;
				cache[line].checker = 1;
				cache[line].TAG = tag;
			}
			else{
				if(cache[line].TAG == tag){ //if position isn't empty and line tag equal cache tag
					hits++;
				}
				else{
					miss++;
					cache[line].TAG = tag;
				}
			}
		}
	}
}

void AssociativeMappingByColumn(int &hits, int &miss, unsigned char memory[100][1000], CacheLine cache[64]){
	unsigned long int addr;
	unsigned long int tag;
	int fifo=0;

	//Accessing the cache by its columns
	for(int i = 0; i<100; i++){	            
			for(int j=0; j<1000; j++){ 		
			addr = (unsigned long int)(&memory[i][j]);
			tag = (addr >> 4);
			bool ToBeSummedUp = true; //Variable to control when add some tag to the cache
			bool firstEmpty = true;

			//Verifying if the tag is in the cache
			for(int k=0; k<64; k++){
				if(cache[k].checker == 1){
					if(cache[k].TAG == tag){
						hits++;
						ToBeSummedUp = false; //While the element isn't in cache, ToBeSummedUp remains true
						break;
					}
				}
			}
			
			//If the tag wasn't found, add the address to the cache and update the tag field
			if(ToBeSummedUp == true){
				miss++;
				cache[fifo].checker = 1;
				cache[fifo].TAG = tag;
				fifo++;
				fifo = fifo%64;
			}
		}
	}
}

void AssociativeMappingByLine(int &hits, int &miss, unsigned char memory[100][1000], CacheLine cache[64]){
	unsigned long int addr;
	unsigned long int tag;
	int fifo=0;

	//Accessing the cache by its lines
	for(int j=0; j<1000; j++){
		for(int i = 0; i<100; i++){			
			addr = (unsigned long int)(&memory[i][j]);
			tag = (addr >> 4);
			bool ToBeSummedUp = true;
			bool firstEmpty = true;

			//Verifying if the tag is in the cache
			for(int k=0; k<64; k++){
				if(cache[k].checker == 1){
					if(cache[k].TAG == tag){
						hits++;
						ToBeSummedUp = false; //While the element isn't in cache, ToBeSummedUp remains true
						break;
					}
				}
			}

			//If the tag wasn't found, add the address to the cache and update the tag field
			if(ToBeSummedUp == true){
				miss++;
				cache[fifo].checker = 1;
				cache[fifo].TAG = tag;
				fifo++;
				fifo = fifo%64;
			}
		}
	}
}

void AssociativeSetMappingByColumn(int &hits, int &miss, unsigned char memory[100][1000], CacheLine cacheToSet[8][8]){
	unsigned long int addr;
	unsigned long int tag;
	unsigned int set;
	int vetFifo[8] = {};

	//Accessing the cache by its columns
	for(int i=0; i<100; i++){
		for(int j=0; j<1000; j++){
			addr = (unsigned long int)(&memory[i][j]);
			set = ((addr & 0b1110000)>>4);
			tag = (addr>>7);
			bool ToBeSummedUp = true;

			//verifying if the tag is in the cache
			for(int k=0; k<8; k++){
				if(cacheToSet[set][k].checker == 1){
					if(cacheToSet[set][k].TAG == tag){
						hits++;
						ToBeSummedUp = false; //While the element isn't in cache, ToBeSummedUp remains true
						break;
					}
				}
			}

			//If the tag wasn't found, add the address to the cache and update the tag field
			if(ToBeSummedUp == true){
				miss++;
				cacheToSet[set][vetFifo[set]].checker = 1;
				cacheToSet[set][vetFifo[set]].TAG = tag;
				vetFifo[set]++;
				vetFifo[set]= vetFifo[set]%8;
			}
		}
	}
}


void AssociativeSetMappingByLine(int &hits, int &miss, unsigned char memory[100][1000], CacheLine cacheToSet[8][8]){
	unsigned long int addr;
	unsigned long int tag;
	unsigned int set;
	int vetFifo[8] = {};

	//Accessing the cache by its columns
	for(int j=0; j<1000; j++){
		for(int i=0; i<100; i++){
			addr = (unsigned long int)(&memory[i][j]);
			set = ((addr & 0b1110000)>>4);
			tag = (addr>>7);
			bool ToBeSummedUp = true;

			//verifying if the tag is in the cache
			for(int k=0; k<8; k++){
				if(cacheToSet[set][k].checker == 1){
					if(cacheToSet[set][k].TAG == tag){
						hits++;
						ToBeSummedUp = false;
						break;
					}
				}
			}

			//If the tag wasn't found, add the address to the cache and update the tag field
			if(ToBeSummedUp == true){
				miss++;
				cacheToSet[set][vetFifo[set]].checker = 1;
				cacheToSet[set][vetFifo[set]].TAG = tag;
				vetFifo[set]++;
				vetFifo[set]= vetFifo[set]%8;
			}
		}
	}
}

void CleanCache(CacheLine cache[64], int &hits, int &miss){
	hits=0;
	miss=0;

	//Setting to zero all the lines
	for(int i=0; i<64; i++){
		cache[i].checker = 0;
	}
}

void CleanCacheSet(CacheLine cache[8][8], int &hits, int &miss){
	hits = 0;
	miss = 0;

	//Setting to zero all the lines
	for(int i=0; i<64; i++){
		cache[0][i].checker = 0;
	}
}

int main(int argc, char const *argv[])
{
	
	//A piece of code that shows how to create 3 children from a single process
	//Note: It reaaaally works!
	srand(time(NULL));
	CacheLine cache[64];
	CacheLine cacheToSet[8][8];
	unsigned char memory[100][1000];
	int hits = 0;
	int miss = 0;

	pid_t child_1, child_2, child_3;

	child_1 = fork();

	if (child_1 == 0) {
		ofstream log1;
    	log1.open ("DirectMapping.txt");
    	log1 << "Type of Mapping: Direct Mapping" << endl << endl;
	    cout << "----- Direct Mapping -----" << endl;

	    DirectMappingByColumn(hits, miss, memory, cache);
		cout << "----- Direct Mapping by Column -----" << endl;
		log1 << "----- Direct Mapping by Column -----" << endl;

		cout << "Hits ratio: " << (double(hits)/(hits+miss))*100 << "%" << endl << endl;
		log1 << "Hits ratio: " << (double(hits)/(hits+miss))*100 << "%" << endl << endl;

		CleanCache(cache, hits, miss);

		DirectMappingByLine(hits, miss, memory, cache);
		cout << "----- Direct Mapping by Line -----" << endl;
		log1 << "----- Direct Mapping by Line -----" << endl;

		cout << "Hits ratio: " << (double(hits)/(hits+miss))*100 << "%" << endl << endl;
		log1 << "Hits ratio: " << (double(hits)/(hits+miss))*100 << "%" << endl << endl;

		log1.close();
	} else {
	    child_2 = fork();

	    if (child_2 == 0)
	    {
	    	ofstream log2;
    		log2.open ("AssociativeMapping.txt");
    		log2 << "Type of Mapping: Completely Associative Mapping\n\n";

	    	cout << "----- Associative Mapping -----" << endl;

	    	AssociativeMappingByColumn(hits, miss, memory, cache);
	    	cout << "----- Associative Mapping by Column -----" << endl;
	    	log2 << "----- Associative Mapping by Column -----\n";

	    	cout << "Hits ratio: " << (double(hits)/(hits+miss))*100 << "%" << endl << endl;
	    	log2 << "Hits ratio: " << (double(hits)/(hits+miss))*100 << "%" << endl << endl;

	    	CleanCache(cache, hits, miss);

	    	AssociativeMappingByLine(hits, miss, memory, cache);
	    	cout << "----- Associative Mapping by Line -----" << endl;
	    	log2 << "----- Associative Mapping by Line -----\n";

	    	cout << "Hits ratio: " << (double(hits)/(hits+miss))*100 << "%" << endl << endl;
	    	log2 << "Hits ratio: " << (double(hits)/(hits+miss))*100 << "%" << endl << endl;

	    	log2.close();
	    } else {
	    	child_3 = fork();

	    	if (child_3 == 0)
	    	{
	    		ofstream log3;
	    		log3.open ("SetAssociativeMapping.txt");
    			log3 << "Type of Mapping: Set Associative Mapping\n\n";

    			cout << "----- Set Associative Mapping -----" << endl;

	    		AssociativeSetMappingByColumn(hits, miss, memory, cacheToSet);
				cout << "----- Set Associative Mapping by Column -----" << endl;
				log3 << "----- Set Associative Mapping by Column -----" << endl;

				cout << "Hits ratio: " << (double(hits)/(hits+miss))*100 << "%" << endl << endl;
				log3 << "Hits ratio: " << (double(hits)/(hits+miss))*100 << "%" << endl << endl;

				CleanCacheSet(cacheToSet, hits, miss);

				AssociativeSetMappingByLine(hits, miss, memory, cacheToSet);
				cout << "----- Set Associative Mapping by Line -----" << endl;
				log3 << "----- Set Associative Mapping by Line -----" << endl;

				cout << "Hits ratio: " << (double(hits)/(hits+miss))*100 << "%" << endl;
				log3 << "Hits ratio: " << (double(hits)/(hits+miss))*100 << "%" << endl << endl;

				log3.close();
	    	} else {
	    		//Parent code
	    		wait(NULL);
	    	}
	    }

	}
	return 0;
}