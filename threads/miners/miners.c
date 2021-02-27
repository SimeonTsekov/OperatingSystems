//------------------------------------------------------------------------
// NAME: Simeon Tsekov
// CLASS: XIa
// NUMBER: 24
// PROBLEM: #1
// FILE NAME: miners.c
// FILE PURPOSE:
// simulates basic economy
//------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t lock;
int treasury = 0;

void* mine(void * arg);
void* trade(void * arg);

//------------------------------------------------------------------------
// FUNCTION: main()
// reads console arguments and creates N miners and N traders, then simulates mining and selling
// PARAMETERS:
// int argc - argument count
// char **argv - values of the arguments
//------------------------------------------------------------------------

int main(int argc, char **argv){
	int miner_count = 1;
	int trader_count = 1;
    	pthread_mutex_init(&lock, NULL);
	if(argc == 3){
		miner_count = atoi(argv[1]);
		trader_count = atoi(argv[2]);
	}

	pthread_t miners[miner_count];
	pthread_t traders[trader_count];

	for(int i=0; i<miner_count; i++){
		pthread_create(&miners[i], NULL, mine, (void*)i);
	}

	for(int i=0; i<trader_count; i++){
		pthread_create(&traders[i], NULL, trade, (void*)i);
	}


	for(int i=0; i<miner_count; i++){
		pthread_join(miners[i], NULL);
	}

	for(int i=0; i<trader_count; i++){
		pthread_join(traders[i], NULL);
	}
	
	printf("Gold: %d", treasury);
}

//------------------------------------------------------------------------
// FUNCTION: mine(void *id)
// the function mines 10 gold
// PARAMETERS:
// void *id - the id of the miner
//------------------------------------------------------------------------
void* mine(void *id){
	for(int i=0; i<20; i++){
		pthread_mutex_lock(&lock);
		treasury += 10;
		pthread_mutex_unlock(&lock);
		printf("Miner %d gathered 10 gold\n", (int)id + 1);
		sleep(2);
	}

	return NULL;
}

//------------------------------------------------------------------------
// FUNCTION: trade(void *id)
// the function tries to sell 10 gold
// PARAMETERS:
// void *id - the id of the trader
//------------------------------------------------------------------------
void* trade(void * id){
	for(int i=0; i<20; i++){
		pthread_mutex_lock(&lock);
		if(treasury > 0){
			treasury -= 10;
			pthread_mutex_unlock(&lock);
			printf("Trader %d sold 10 gold\n", (int)id + 1);
		} else {
			pthread_mutex_unlock(&lock);
			printf("The warehouse is empty, cannot sell!\n");
		}
		sleep(2);
	}
	return NULL;
}
