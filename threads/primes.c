//------------------------------------------------------------------------
// NAME: Simeon Tsekov
// CLASS: XIa
// NUMBER: 24
// PROBLEM: #1
// FILE NAME: primes.c
// FILE PURPOSE:
// outputs the number of prime numbers within a certain range
//------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int is_prime(int n);
void *calculate(void *arg);

//------------------------------------------------------------------------
// FUNCTION: main()
// the function reads the input and uses the other functions
// PARAMETERS:
//------------------------------------------------------------------------

int main(){
	char buffer[300];
	pthread_t threads[100];
	int thread_count = 0;

	while(1){
		fgets(buffer, 299, stdin);
		if(buffer[0] == 'e' && buffer[1] == '\n'){
			for(int i=0; i<thread_count; i++){
				pthread_join(threads[i], NULL);
			}
			break;
		} else if(buffer[0] == 'p' && (atoi(&buffer[2])>=0)){
			pthread_create(&threads[thread_count++], NULL, calculate, (int*)atoi(&buffer[2]));
		} else {
			printf("Supported commands:\np N - Starts a new calculation for the number of primes from 1 to N\ne - Waits for all calculations to finish and exits\n");
		}
	}
	return 0;
}

//------------------------------------------------------------------------
// FUNCTION: is_prime()
// checks if a number is prime
// PARAMETERS:
// n - the number to check
//------------------------------------------------------------------------

int is_prime(int n){
     if (n <= 1) return 0;
     if (n % 2 == 0 && n > 2) return 0;
     for(int i = 3; i < n / 2; i+= 2)
     {
         if (n % i == 0)
             return 0;
     }
     return 1;	
}

//------------------------------------------------------------------------
// FUNCTION: *calculate()
// finds the amount of prime numbers within a certain number
// PARAMETERS:
// *arg - the number, within which the calculations occur, after being casted to int
//------------------------------------------------------------------------

void *calculate(void *arg){
	printf("Prime calculation started for N=%d\n", (int)arg);

	int c = 0;
	for(int i=2; i<(int)arg; i++){
		if(is_prime(i)){
			c++;
		}
	}
	
	printf("Number of primes for N=%d is %d\n", (int)arg, c);

	return 0;
}
