#include "header.h"

/* Lock the semaphore n of the semaphore set semaph */

void Wait(int semaph, int n)
{
  struct sembuf sop;
  sop.sem_num = n;
  sop.sem_op =  -1;
  sop.sem_flg = 0;
  semop(semaph,&sop,1);
}

/* Unlock the semaphore n of the semaphore set semaph */
void Signal(int semaph, int n)
{
  struct sembuf sop;
  sop.sem_num = n;
  sop.sem_op =  1;
  sop.sem_flg = 0;
  semop(semaph,&sop,1);
}

/* make an array of n semaphores with key k */
int GetSemaphs(key_t k, int n)
{
  int semid, i;

  /* get a set of n semaphores with the given key k */
  
  if ((semid=semget(k,n,IPC_CREAT|0666))!= -1){
    for (i=0;i<n;i++)
      Signal(semid,i); /* unlock all the semaphores */
  }
  return semid;
}

void read_line(char* line, int size){
	if ( fgets(line, size, stdin) ){
		char *newline = strchr(line, '\n'); /* check for trailing '\n' */
		if ( newline )	{
			*newline = '\0'; /* overwrite the '\n' with a terminating null */
		}
	}
}
