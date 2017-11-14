#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#define UNEMPLOYED 1
#define EMPLOYED 0

#define MAX_NUMBER_OF_JOBS 4

void Work(int PayPeriods) {
	sleep(PayPeriods);
}

void ANewDay() {
	sleep(4);
}

// 25% chance of something bad happening
int LifeHappens() {
	if (rand() % 4) return 0;
	else return 1;
}

// 5% chance of losing job
int MaintainJob() {
	if (rand() % 20) return 1;
	else return 0;
}

int CouldNotFindJob(int rejected) {
	if (rejected) return UNEMPLOYED;
	else return EMPLOYED;
}

int PickJob() {
	return rand() & 4;
}

int GetJobPayPeriod() {
	return rand() % 2;
}

int Age(int seconds) {
	return seconds / 1456;
}

// 0.01% of having a heart attack
int HeartAttack() {
	if (rand() % 1000) return 0;
	else return 1;
}