#include "header.h"

int main(){
	/*
	Change.c - A writer
	========
	*/

	int db_id, read_id, sema_set;
	struct student * db;
	int * read_count;
	char line[256];
	
	struct student * current_student;

	int student_id;
	char new_name[20];
	char new_addr[256];
	char new_tele[16];

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

	//Prompt the user for password;
	printf("Please enter the password %d: ", sizeof(line));
	read_line(line,sizeof(line),stdin);

	//Validate the password - just check if the user types 000;
	if( strcmp(line,PASSWORD) == 0 ){
		//Get the id of the student the user wants to modify;
		printf("Please enter id of student: ");
		fgets(line,sizeof(line),stdin);
		student_id = atoi(line);

		//Get the new information about the student;
		printf("Please enter new name: ");
		read_line(new_name,sizeof(new_name),stdin);
		
		printf("Please enter new address: ");
		read_line(new_addr,sizeof(new_addr),stdin);

		printf("Please enter new telephone: ");
		read_line(new_tele,sizeof(new_tele),stdin);

		Wait(sema_set, 0); 
			//Search the shared memory for the student record with
			//the student id typed by the user;
			int index = -1;
			int i;
			for(i = 0; i < DATABASE_SIZE && index == -1; i++){
				if( db[i].id != 0 ){
					if( db[i].id == student_id ){
						current_student = &db[i];
						index = i;
					}
				}else{
					current_student = &db[i];
					printf("Student not found, but there is empty space.  Adding student to database.");
					index = i;
				}
			}

			if( index != -1 ){
				//update the student record with new information;
				current_student->id = student_id;
				strcpy(current_student->name,new_name);
				strcpy(current_student->address,new_addr);
				strcpy(current_student->telephone,new_tele);
			}else{
				//if the id is not found, print an error message;
				printf("No student in the database for id '%d', and no space to add one.\n",student_id);
			}
			sleep(10); // for debugging and testing
		Signal(sema_set, 0);
	}else{
		printf("Incorrect password, aborting.\n");
	}
}


