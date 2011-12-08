#ifndef RNOLAN_HEADER_H
#define RNOLAN_HEADER_H

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <string.h>

struct student{
	char name[20];
	int id;
	char address[256];
	char telephone[16];
};

#define DB_KEY ((key_t)0x667)			// key to access shared mem
#define READ_KEY ((key_t)0x668)			// key to access read_count
#define SEMAPHORE_KEY ((key_t)0x669)	// key to access semaphore set
#define DATABASE_SIZE 20
#define DATABASE_FILENAME "rnolan_database.dat"
#define PASSWORD "000"

void Wait(int semaph, int n);
void Signal(int semaph, int n);
int GetSemaphs(key_t k, int n);
void read_line(char* line, int size, FILE* read_from);

#endif
