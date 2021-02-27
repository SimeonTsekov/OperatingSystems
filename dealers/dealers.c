#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#define N 20
#define M 5

pthread_mutex_t cars[M];
void *test(void *arg);

int main(){
	pthread_t drivers[N];
	
	for(int i=0; i<M; i++){
		pthread_mutex_init(&cars[i], NULL);
	}

	for(int i=0; i<N; i++){
		pthread_create(&drivers[i], NULL, test, (int*)i);
	}

	for(int i=0; i<N; i++){
		pthread_join(drivers[i], NULL);
	}

	for(int i=0; i<M; i++){
		pthread_mutex_destroy(&cars[i]);
	}

	return 0;
}

void *test(void *id){
	while(1){
		for(int i=0; i<M; i++){
			if(pthread_mutex_trylock(&cars[i])==0){
				printf("Buyer %d takes car %d.\n", (int)id+1, i+1);
				sleep(1);
				printf("Buyer %d returns car %d.\n", (int)id+1, i+1);
				pthread_mutex_unlock(&cars[i]);
				return 0;
			}
		}
	}
}
