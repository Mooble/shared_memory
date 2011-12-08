#include "header.h"

int main(int argc, char **argv){
	//Clean.c - A writer
	//=======
	int db_id, read_id, sema_set;
	struct student * db;
	int * read_count;
	FILE *file;
	char filename[256];
	int delete_sema = 0;

	if(argc > 1){
		strcpy(filename,argv[1]);
	}else{
		printf("No filename given, defaulting to %s\n",DATABASE_FILENAME);
		strcpy(filename,DATABASE_FILENAME);
	}

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

	Wait(sema_set, 0); // assuming semaset is the id of the semaphore set 
		if( file = fopen(DATABASE_FILENAME,"w") ){		
			//write the contents of the shared memory to file in the format of the input file;
			int i;
			for(i = 0; i < DATABASE_SIZE; i++){
				fprintf(file,"%s\n",db[i].name);
				fprintf(file,"%d\n",db[i].id);
				fprintf(file,"%s\n",db[i].address);
				fprintf(file,"%s\n",db[i].telephone);
			}
			
			//delete the shared memory segments
			shmctl(db_id, IPC_RMID,(struct shmid_ds *)0);				
			delete_sema = 1;
		}else{
			printf("Could not open file for writing.  Cleanup canceled.\n");
		}

		sleep(10); // for debugging and testing
	Signal(sema_set, 0);

	//delete the semaphores.
	if(delete_sema == 1){
		semctl(sema_set,0,IPC_RMID);
	}
}
