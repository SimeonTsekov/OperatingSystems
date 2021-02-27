//--------------------------------------------
// NAME: Simeon Tsekov
// CLASS: XIa
// NUMBER: 24
// PROBLEM: #3
// FILE NAME: sc3.c
// FILE PURPOSE:
// A prototype of Blizzard's third installment in the StarCraft series.
// The player begins with 5 workers and gives the number of mineral blocks as a console argument.
// The goal is to create 20 soliders.
// With the key 'm' a solider is created and with 's' a worker is created.
//---------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#define STARTING_PROBE_COUNT 5
#define INF 200
#define UNIT_THRESHOLD 200
#define PROBE_PRICE 50
#define ZEALOT_PRICE 50
#define ZEALOT_THRESHOLD 20
#define MINERAL_THRESHOLD 8


typedef struct mineral_block_t{
	unsigned int minerals;
	pthread_mutex_t block_lock;
}mineral_block_t;

int block_count = 0;
mineral_block_t *mineral_blocks = NULL;

int nexus = 0;
pthread_mutex_t nexus_lock = PTHREAD_MUTEX_INITIALIZER; 


void *mine(void *block_count);
int parse_command(int argc, char **argv);
void block_init();
void block_destroy();
void print_info(int probe_count, int zealot_count);
int extract_minerals(int probe_id, int block_id);
void transport_minerals(int probe_id, int carried_minerals);


//--------------------------------------------
// FUNCTION: main
// Responsible for the creation and destruction of the threads and mutexes, as well as the input cycle.
// PARAMETERS:
// int argc - argument count
// char **argv - 2D array of the console arguments
//----------------------------------------------
int main(int argc, char **argv){
	int zealot_count = 0;
	int probe_count = STARTING_PROBE_COUNT;
	pthread_t probes[INF];
	
	block_count = parse_command(argc, argv);
	
	//initialization of the blocks
	block_init();

	//creation of the workers
	for(int i=0; i<probe_count; i++){
		if(pthread_create(&probes[i], NULL, mine, (void*)(i+1))!=0){
			perror("pthread_create");
			return 0;
		}
	}
	
	while(1){
		int total_count = zealot_count + probe_count;
		
		if(total_count<UNIT_THRESHOLD){
			char input = getchar();

			switch(input){
				case 'm':
					pthread_mutex_lock(&nexus_lock);	
					if(nexus >= ZEALOT_PRICE){
						nexus -= ZEALOT_PRICE;
						sleep(1);
						printf("You wanna piece of me, boy?\n");
						zealot_count++;
					}else{
						printf("Not enough minerals.\n");
					}
					pthread_mutex_unlock(&nexus_lock);
					break;
				case 's':
					pthread_mutex_lock(&nexus_lock);
					if(nexus >= PROBE_PRICE){
						nexus -= PROBE_PRICE;
						printf("SCV good to go, sir.\n");
						probe_count++;
						sleep(4);
						if(pthread_create(&probes[probe_count], NULL, mine, (void*)(probe_count+1))!=0){
							perror("pthread_create");
							return 0;
						}
					} else {
						printf("Not enough minerals.\n");
					}
					pthread_mutex_unlock(&nexus_lock);
					break;
			}
			if(zealot_count == ZEALOT_THRESHOLD){
				break;
			}
		} else {
			printf("Exceeded unit capacity of %d\n", UNIT_THRESHOLD);
		}	
	}

	//destroying the workers
	for(int i=0; i<probe_count; i++){
		if(pthread_join(probes[i], NULL)!=0){
			perror("pthread_join");
			return 0;
		}
	}
	
	//printing of the overall info
	print_info(probe_count, zealot_count);

	//destroying of the mineral blocks
	block_destroy();

	//destroying of the command center
	pthread_mutex_destroy(&nexus_lock);

	return 0;
}


//--------------------------------------------
// FUNCTION: mine
// This function represents the routine of the worker. It is responsible for the mining, as well as the synchronization of the threads.
// PARAMETERS:
// void *probe_id - a void pointer to the id of the worker, that's mining.
//----------------------------------------------
void *mine(void *probe_id){
	int id = (int)probe_id;
	int mined = 0;
	int existing = 0;

	while(1){
		for(int i=0; i<block_count; i++){
			sleep(3);

			if(pthread_mutex_trylock(&mineral_blocks[i].block_lock)==0){
				if(mineral_blocks[i].minerals>0){
					existing++;
					mined = extract_minerals(id, i);
					pthread_mutex_unlock(&mineral_blocks[i].block_lock);
					transport_minerals(id, mined);
				}else{
					pthread_mutex_unlock(&mineral_blocks[i].block_lock);	
				}
			}else{
				existing++;
			}
		}
		
		if(!existing){
			return 0;
		}

		existing = 0;
	}
}


//--------------------------------------------
// FUNCTION: block_init
// This function initializes the mutexes of the mineral blocks and sets the value of the minerals to 500.
// PARAMETERS:
// -
//----------------------------------------------
void block_init(){
	mineral_blocks = malloc(sizeof(mineral_block_t) * block_count);
	
	for(int i=0; i<block_count; i++){
		pthread_mutex_init(&mineral_blocks[i].block_lock, NULL);
		mineral_blocks[i].minerals = 500;
	}

}


//--------------------------------------------
// FUNCTION: block_destroy
// This function destroys the mineral block mutexes and frees the array.
// PARAMETERS:
// -
//----------------------------------------------
void block_destroy(){
	for(int i=0; i<block_count; i++){
		pthread_mutex_destroy(&mineral_blocks[i].block_lock);
	}
	
	free(mineral_blocks);
}


//--------------------------------------------
// FUNCTION: parse_command
// This function parses the console arguments.
// PARAMETERS:
// int argc - argument count
// char **argv - argument values
//----------------------------------------------
int parse_command(int argc, char **argv){
	if(argc == 1){
		return 2;
	}
	
	return atoi(argv[1]);
}


//--------------------------------------------
// FUNCTION: print_info
// This function prints the final info of the game.
// PARAMETERS:
// int probe_count - worker count
// int zealot_count -solider count
//----------------------------------------------
void print_info(int probe_count, int zealot_count){
	printf("Map minerals %d, player minerals %d, SCVs %d, Marines %d\n", 
			block_count*500, 
			nexus, 
			probe_count, 
			zealot_count);
}


//--------------------------------------------
// FUNCTION: extract_minerals
// This function extracts minerals from the blocks.
// PARAMETERS:
// int probe_id - id of the worker
// int block_id - id of the block
//----------------------------------------------
int extract_minerals(int probe_id, int block_id){
	int mined = 0;

	printf("SCV %d is mining from mineral block %d\n", probe_id, block_id+1);

	if(mineral_blocks[block_id].minerals>=MINERAL_THRESHOLD){
		mined = MINERAL_THRESHOLD;
		mineral_blocks[block_id].minerals -= MINERAL_THRESHOLD;
	}else{
		mined = mineral_blocks[block_id].minerals;
		mineral_blocks[block_id].minerals = 0;
	}

	return mined;
}


//--------------------------------------------
// FUNCTION: transport_minerals
// This function transports the minerals to the command center.
// PARAMETERS:
// int probe_id - id of the worker
// int carriedd_minerals - the quantity of the minerals carried
//----------------------------------------------
void transport_minerals(int probe_id, int carried_minerals){
	printf("SCV %d is transporting minerals\n", probe_id);
	sleep(2);

	printf("SCV %d delivered minerals to the Command center\n", probe_id);
	pthread_mutex_lock(&nexus_lock);
	nexus += carried_minerals;
	pthread_mutex_unlock(&nexus_lock);
}
