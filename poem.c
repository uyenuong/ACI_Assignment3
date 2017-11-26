#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

#include "WorkFunctions.h"

#define ALIVE 1

#define MINIMUM_WAGE 7.25
#define MAX_NUMBER_OF_JOBS 4

#define UNEMPLOYED 1
#define EMPLOYED 0
#define RETIRED 0
#define AMOUNT_TO_RETIRE 1000000

#define DEAD 1

// Important things to remember!!
pthread_mutex_t bookKeeping = PTHREAD_MUTEX_INITIALIZER; 
pthread_t jobs[MAX_NUMBER_OF_JOBS];

// Starting off with nothing...
float savings = 0;
int numberOfJobs = 0;

// Updating my list of jobs
void LostJob() {
	int index;
	for (int i = 0; i < numberOfJobs; i++) {
		if (jobs[i] == pthread_self()) {
			index = i;
		}
	}
	for (int i = index + 1; i < numberOfJobs; i++) {
		jobs[i-1] = jobs[i];
	}
	numberOfJobs -= 1;
}

// Working at a job
void* GetAJob(void* jobTitle) {
	// I got a job!
	pthread_mutex_lock( &bookKeeping );     
	numberOfJobs += 1;
	pthread_mutex_unlock( &bookKeeping );
	printf("I got job %s. Number of jobs: %d\n", (char*) jobTitle, numberOfJobs);

	int PayPeriods = GetJobPayPeriod();

	// Working...
	while (MaintainJob()) {

		Work(PayPeriods); 

		// Access my records to add money to my savings
		pthread_mutex_lock( &bookKeeping );     
		savings += MINIMUM_WAGE;
		pthread_mutex_unlock( &bookKeeping );

		if (savings == AMOUNT_TO_RETIRE) {
			printf("I have made enough money to retire!\n");
			return NULL;
		}
	}

	// I lost my job! :*(
	pthread_mutex_lock( &bookKeeping );     
	LostJob();
	pthread_mutex_unlock( &bookKeeping );
	printf("I lost my job %s!\n", (char*) jobTitle);

	return NULL;
}

int main(int argc, char const *argv[])
{
	// Life's randomness begins
	srand(time(NULL));

	// Becoming an adult...
	time_t beginWorking = time(0);

	char* availableJobs[] = {
		"JANITOR",
		"DUNKIN' DONUTS CREW MEMBER",
		"WAITRESS",
		"RETAILS SALES REPRESENTATIVE",
	};

	while(ALIVE) {

		if (numberOfJobs == 0 || numberOfJobs < MAX_NUMBER_OF_JOBS){

			// I need to find a job!
			printf("Looking for a job...\n");
			int jobSearch = pthread_create( &jobs[numberOfJobs], NULL, GetAJob, availableJobs[PickJob()] );

			if (CouldNotFindJob(jobSearch)) {
				fprintf( stderr, "Could not find job! Now unemployed :( \n" );
		    	return UNEMPLOYED;
			}

		}

		if (LifeHappens()) {
			// Gotta pay some bills
			int bill = rand() % 500;
			savings -= bill;

			printf("Life sucks...gotta pay $%d\n", bill);
		}

		if (HeartAttack()) {
			// I had a heart attack and died
			printf("I DIED FROM OVERWORKING!\n");
			return DEAD;
		}

		if (savings == AMOUNT_TO_RETIRE) {
			// I made enough money to retire!
			printf("HALLLUJAH I AM RETIRING! :D Age %d\n", Age(difftime(time(0), beginWorking)) );
			break;
		}

		printf("Current savings balance: $%.2f\n", savings);

		ANewDay();
	}

	// Quitting all jobs 
	for ( int i = 0 ; i < numberOfJobs ; i++ ) {
		pthread_join( jobs[i], NULL );
	}

	return RETIRED;

}