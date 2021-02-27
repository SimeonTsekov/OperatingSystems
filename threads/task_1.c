#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *work(void *arg);

int main(){
	pthread_t threads[10];
	for(int i=0; i<10; i++){
		int *p = malloc(sizeof(int));
		*p = i;
		pthread_create(&threads[i], NULL, work, p);
	}
	for(int i=0; i<10; i++){
		void *res;
		pthread_join(threads[i], &res);
		printf("Joined %d\n", (int)res);
	}
	return 0;
}

void *work(void *arg){
	int id = *((int *)arg);
	printf("Hello from thread %d\n", id);
	free(arg);
	return (void *)(id * 2);
}
