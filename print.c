#include "header.h"

int main(){
	/*
	Print.c - A reader
	=======
	*/

	int db_id, read_id, sema_set;
	struct student * db;
	int * read_count;
	char line[256];
	
	int student_id;

	//Get the id of the shared memory segments created for storing data;
	//Get shared memory seg for database
	db_id = shmget(DB_KEY,0,0);
	if( db_id <0 ){
		perror("Failed to retrieve db");
		exit(1);
	}

	//create shared memory for read_count
	read_id = shmget(READ_KEY,0,0);
	if( read_id < 0 ){
		perror("Failed to retrieve read_count");
		exit(1);
	}
	
	// Get two semaphores
	sema_set = semget(SEMAPHORE_KEY,0,0);
	if (sema_set < 0 ){
		perror("Failed to retrieve semaphores");
		exit(2);
	} 

	//Attach the shared memory segments;
	read_count = (int *)shmat(read_id,0,0);
	db = (struct student *)shmat(db_id,0,0);


	Wait(sema_set,1);
		//increment read_count; // in shared memory
		(*read_count) = *read_count + 1;

		//if value of read_count==1 then Wait(semaset, 0);
		if( *read_count == 1 ){
			Wait(sema_set,0);
		}
	Signal(sema_set,1);

	//Print entire database to screen
	int i;
	for(i = 0; i < DATABASE_SIZE; i++){
		printf("-----\n");
		printf("\tID: %d\n",db[i].id);
		printf("\tName: %s\n",db[i].name);
		printf("\tAddress: %s\n",db[i].address);
		printf("\tTelephone: %s\n",db[i].telephone);
		printf("-----\n");
	}

	Wait(sema_set,1);
		//deccrement read_count; // in shared memory
		*read_count = *read_count - 1;
		//if value of read_count==0 then Signal(semaset, 0);
		if(*read_count == 0){
			Signal(sema_set,0);
		}
	Signal(sema_set,1);
}
