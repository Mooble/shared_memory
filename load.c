#include "header.h"


int main(int argc, char **argv){
	int db_id;
	int read_id;
	struct student *data;
	char * line;
	FILE *file;
	int loaded_file = 0;
	int sema_set;
	int * read_count;

	//Create shared memory seg for database
	db_id = shmget(DB_KEY,sizeof(struct student)*DATABASE_SIZE,IPC_CREAT|0666);
	if( db_id <0 ){
		perror("Couldn't create db");
		exit(1);
	}

	//create shared memory for read_count
	read_id = shmget(READ_KEY,sizeof(int),IPC_CREAT|0666);
	if( read_id < 0 ){
		perror("Couldn't create read_count");
		exit(1);
	}
	
	// Create two semaphores
	sema_set = GetSemaphs(SEMAPHORE_KEY, 2);
	if ( sema_set < 0 ){
		perror("Couldn't create semaphores");
		exit(2);
	}

	//Initialize data
	Wait(sema_set,0);
		//read_count
		read_count = (int *)shmat(read_id,0,0);
		*read_count = 0;

		//db
		data = (struct student *)shmat(db_id,0,0);

		if(file = fopen(DATABASE_FILENAME,"rt")){
			fread(data, sizeof(struct student)*DATABASE_SIZE, 1, file);
			loaded_file = 1;
		}else{
			int i;
			for(i = 0; i < DATABASE_SIZE; i++){
				data[i].id = 0;
			}
		}

		if(loaded_file == 0){
			printf("No database file, or it was corrupt.  Database initialized to be empty.");
		}

		sleep(10);
	Signal(sema_set,0);

	return 0;
}
