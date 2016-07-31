/*
 * Parallel using a mutex program for liked list implementation.
 * 
 * W.A Samadhi P Kumarasinghe
 * 
 * Compile the program : gcc -g -Wall -o pthreadMutex pthreadMutex.c
 * Run the program     : ./pthreadMutex <n> <m> <thread_count> <mMember> <mInsert> <mDelete>
 *
 * */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

#define MAX_THREADS 1024

//Variables for number of nodes, number of random operations, the number of member operations, inserts and deletes carried out so far
int n=0, m=0, memCnt = 0,insCnt = 0,delCnt = 0;

//A variable to store the tread count
long thread_count=0;

//Variables to store the fractions of member operations, inserts and deleted expected to be performed
float mIns = 0.0,mDel = 0.0,mMem = 0.0,mInsert = 0.0,mDelete = 0.0,mMember = 0.0;

//Initialize the head
struct list_node_s *head = NULL;

//Mutex declaration
pthread_mutex_t mutex;

//Node structure definition
struct list_node_s{
  int data;
  struct list_node_s* next;
};

//The set of functions
void genOperations(int argc, char *argv[]);
void populate(struct list_node_s *head);
void *Thread_Operation();
int Member(int value, struct list_node_s* head_p);
int Insert(int value, struct list_node_s** head_pp);
int Delete(int value, struct list_node_s*** head_pp);

int main(int argc, char* argv[]){

    pthread_t *thread_handlers;
    thread_handlers = malloc(sizeof(pthread_t) * thread_count);   //Allocate memory for the threads
    struct timeval startTime, endTime;                            //Variables to store the start time and the termination time
    float elapsedTime;                                            //Variable to store the total time consumed as a float

    genOperations(argc, argv);                                    //Generate the fractions mMem, mIns and mDel

    mInsert= mIns * m;                                            //Compute the number of insertions as mIns fraction of total number of operations(m)
    mDelete = mDel * m;                                           //Compute the number of deletions as mDel fraction of total number of operations(m)
    mMember = mMem * m;                                           //Compute the number of member operations as mMem fraction of total number of operations(m)

    populate(head);
    pthread_mutex_init(&mutex, NULL);                             //Initialize the mutex
    
    gettimeofday(&startTime, NULL);                               //start timing

    int i=0;
    while(i<thread_count){                                        //loop to create the given number of threads
        pthread_create(&thread_handlers[i],NULL, (void *)Thread_Operation,NULL);
        i++;
    }

    i=0;
    while(i<thread_count){                                        //loop to implement thread join operation
        pthread_join(thread_handlers[i],NULL);
        i++;
    }

    gettimeofday(&endTime, NULL);                                 //End timing

    pthread_mutex_destroy(&mutex);                                //Destroy the mutex

    elapsedTime=(double) (endTime.tv_usec - startTime.tv_usec) / 1000000 + (double) (endTime.tv_sec - startTime.tv_sec);
    printf("Elapsed time: %f\n",elapsedTime);

    return 0;
}

/*A function to generate the operations mMember, mInsert and mDelete. Gives error messages if the inputs are missing or in the wrong format*/
void genOperations(int argc, char *argv[]){
    if (argc != 7) {
        printf("One or more arguement is missing!\n");            //Notify the lack of arguements
        exit(0);                                                  //Exit the program due to lack of arguements
    }

    n = (int) strtol(argv[1], (char **) NULL, 10);                //Get the number of initial insertions fromarguement 1
    m = (int) strtol(argv[2], (char **) NULL, 10);                //get the number of operations from arguement 2
    thread_count = (long) strtol(argv[3], (char **) NULL, 10);    //get the thred count from arguement 3

    mMem = (float) atof(argv[4]);                                 //Get the fraction of member operations from arguement 4
    mIns = (float) atof(argv[5]);                                 //Get the fraction of insert operations from arguement 5
    mDel = (float) atof(argv[6]);                                 //Get the fraction of delete operations from arguement 6

    if (n <= 0 || m <= 0 || mMem + mIns + mDel != 1.0) {          //If the sum of fractions is not equal to one
        printf("Invalid arguements!\n");                          //Notify about the invalid arguements
        exit(0);                                                  //Exit the program due to invalid arguements
    }
}

/*A function to populate the linked list to a given number of unique random elements*/
void populate(struct list_node_s *head){
    int i = 0;
    while (i < n) {
        if (Insert(rand() % 65535, &head) == 1)                   //If the insertion was successful
            i++;
    }
}

/*A function to implemet the operations by threads*/
void *Thread_Operation(){
    int totCnt = 0;                                              //store the total number of operations performed so far locally

    while (totCnt < m) {                                          //Loop to inplement operations up to the number of operations expected

        int randVal = rand() % 65535;                             //Generate a random value

        if ((randVal%3) == 0) {                                   //If the value is divisible by 3 assign it as a member operation
            pthread_mutex_lock(&mutex);                           //Enter the critical section
            if(memCnt< mMember){                                  //Check if the number of member operations are over
            	Member(randVal , head);                           //Perform the member operation
		        memCnt++;                                         //Increment the count of member operations performed
            }
            pthread_mutex_unlock(&mutex);                         //Exit the critical section
        }

        else if ((randVal%3) == 1) {                              //If there is a remainder of one after dividing by 3, assign it as an Insert operation
            pthread_mutex_lock(&mutex);                           //Enter the critical section
            if(insCnt< mInsert){                                  //Check if the number of insertions are over
            	Insert(randVal , &head);                          //Perform the insert operation
		        insCnt++;                                         //Increment the count of insert operations performed
            }
            pthread_mutex_unlock(&mutex);                         //Exit the critical section
        }

        else if ((randVal%3)== 2) {                               //If there is a remainder of two after dividing by 3, assign it as an Delete operation   
            pthread_mutex_lock(&mutex);                           //Enter the critical section
            if(delCnt < mDelete){                                 //Check if the number of deletions are over
            	Delete(randVal , &head);                          //Perform the delete operation
		        delCnt++;                                         //Increment the count of delete operations performed
            }
            pthread_mutex_unlock(&mutex);                         //Exit the critical section
        }

	totCnt=memCnt+insCnt+delCnt;
    }
    return NULL;
}

/*A function to search for a member*/
int Member(int value, struct list_node_s* head_p){
	struct list_node_s* curr_p=head_p;

	while(curr_p!=NULL && curr_p->data<value)
    		curr_p=curr_p->next;

	if(curr_p==NULL || curr_p->data>value){
		return 0;
	}

	else{
		return 1;
	}
}

/*A function to insert a value*/
int Insert(int value, struct list_node_s** head_pp){
	struct list_node_s* curr_p=*head_pp;
	struct list_node_s* pred_p=NULL;
	struct list_node_s* temp_p;

	while(curr_p!=NULL && curr_p->data<value){
		pred_p=curr_p;
		curr_p=curr_p->next;
	}

	if(curr_p==NULL || curr_p->data>value){
		temp_p=malloc(sizeof(struct list_node_s));
		temp_p->data=value;
		temp_p->next=curr_p;
		if(pred_p == NULL)                                   //  New first node 
			*head_pp=temp_p;
		else
			pred_p->next=temp_p;
		return 1;
	}
	else{                                                // Value already in list 
		return 0;
	}
}

/*A function to delete a value*/
int Delete(int value, struct list_node_s*** head_pp){
	struct list_node_s* curr_p=*head_pp;
	struct list_node_s* pred_p=NULL;

	while(curr_p!=NULL && curr_p->data<value){
		pred_p=curr_p;
		curr_p=curr_p->next;
	}

	if(curr_p!=NULL && curr_p->data==value){
		if(pred_p==NULL){                                  //Deleting first node in list 
			*head_pp=curr_p->next;
			free(curr_p);
		}
		else{
			pred_p->next=curr_p->next;
			free(curr_p);
		}
		return 1;
	}
	else{                                                  // Value isn’t in list 
		return 0;
	}
}















































































