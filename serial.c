/*
 * Serial program for liked list implementation.
 * 
 * W.A Samadhi P Kumarasinghe
 * 
 * Compile the program : gcc -g -Wall -o serial serial.c
 * Run the program     : ./serial <n> <m> <mMember> <mInsert> <mDelete>
 *
 * */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

//Variables for number of nodes and number of random operations
int n, m;

//Variables for fractions of operations for each type of operation
float mIns,mDel,mMem;

//Node structure definition
struct list_node_s {
  int data;
  struct list_node_s* next;
};

//The set of functions
int Member( int value, struct  list_node_s* head_p);
int Insert(int value, struct list_node_s** head_pp);
int Delete(int value, struct list_node_s** head_pp);
void genOperations(int argc, char *argv[]);
void populate(struct list_node_s *head);

int main(int argc, char *argv[]){

    struct list_node_s *head = NULL;                //Initialize the head
    struct timeval startTime, endTime;              //Variables to store the start time and the termination time
    float elapsedTime;                              //Variable to store the total time consumed as a float

    genOperations(argc, argv);                      //Generate the fractions mMem, mIns and mDel
    populate(head);

    int totCnt = 0;                                 //Initialize a variable to store the count on the number of operations performed so far
    int memCnt = 0;                                 //Initialize a variable to store the count on the number of Member operations performed so far
    int insCnt = 0;                                 //Initialize a variable to store the count on the number of Insert operations performed so far
    int delCnt = 0;                                 //Initialize a variable to store the count on the number of Delete operations performed so far

    float mInsert= mIns * m;                        //Compute the number of insertions as mIns fraction of total number of operations(m)
    float mDelete = mDel * m;                       //Compute the number of deletions as mDel fraction of total number of operations(m)
    float mMember = mMem * m;                       //Compute the number of member operations as mMem fraction of total number of operations(m)

    gettimeofday(&startTime, NULL);                 //start timing

    while (totCnt < m) {                            //Loop to inplement operations up to the number of operations expected
      int randVal = rand() % 65535;                 //Generate a random value

      if ((randVal%3) == 0 && memCnt< mMember) {    //If the value is divisible by 3 assign it as a member operation
        Member(randVal , head);                     //Perform the member operation
        memCnt++;                                   //Increment the count of member operations performed
      }

      else if ((randVal%3) == 1 && insCnt< mInsert) {  //If there is a remainder of one after dividing by 3, assign it as an Insert operation
        Insert(randVal , &head);                       //Perform the insert operation
        insCnt++;                                      //Increment the count of insert operations performed
      }

      else if ((randVal%3)== 2 && delCnt < mDelete) {  //If there is a remainder of two after dividing by 3, assign it as an Delete operation   
        Delete(randVal , &head);                       //Perform the delete operation
        delCnt++;                                      //Increment the count of delete operations performed
      }

      totCnt++;
    }

    gettimeofday(&endTime, NULL);                   //End timing
    elapsedTime=(double) (endTime.tv_usec - startTime.tv_usec) / 1000000 + (double) (endTime.tv_sec - startTime.tv_sec);
    printf("Elapsed time: %f\n",elapsedTime);

}

/*A function to generate the operations mMember, mInsert and mDelete. Gives error messages if the inputs are missing or in the wrong format*/
void genOperations(int argc, char *argv[]){
    if (argc != 6) {
        printf("One or more arguement is missing!\n");    //Notify the lack of arguements
        exit(0);                                          //Exit the program due to lack of arguements
    }

    n = (int) strtol(argv[1], (char **) NULL, 10);        //Get the number of initial insertions fromarguement 1
    m = (int) strtol(argv[2], (char **) NULL, 10);        //get the number of operations from arguement 2

    mMem = (float) atof(argv[3]);                         //Get the fraction of member operations from arguement 3
    mIns = (float) atof(argv[4]);                         //Get the fraction of insert operations from arguement 3
    mDel = (float) atof(argv[5]);                         //Get the fraction of delete operations from arguement 3

    if (n <= 0 || m <= 0 || mMem + mIns + mDel != 1.0) {  //If the sum of fractions is not equal to one
        printf("Invalid arguements!\n");                  //Notify about the invalid arguements
        exit(0);                                          //Exit the program due to invalid arguements
    }
}

/*A function to populate the linked list to a given number of unique random elements*/
void populate(struct list_node_s *head){
    int i = 0;
    while (i < n) {
        if (Insert(rand() % 65535, &head) == 1)           //If the insertion was successful
            i++;
    }
}

/*A function to search for a member*/
int Member(int value, struct list_node_s* head_p) {
    struct list_node_s* curr_p = head_p;
    while (curr_p != NULL && curr_p -> data < value){
      curr_p = curr_p->next;
    }
    if (curr_p == NULL || curr_p->data > value) {
      return 0;
    } else {
      return 1;
    }
}

/*A function to insert a value*/
int Insert(int value, struct list_node_s** head_p) {
  struct list_node_s* curr_p = *head_p;
  struct list_node_s* pred_p= NULL;
  struct list_node_s* temp_p;
  
  while (curr_p != NULL && curr_p-> data < value) {
    pred_p = curr_p;
    curr_p = curr_p->next;
  }
  
  if (curr_p == NULL || curr_p-> data > value) {
    temp_p = malloc(sizeof(struct list_node_s));
    temp_p -> data = value;
    temp_p -> next = curr_p;
    if (pred_p == NULL)                                   //  New first node 
      *head_p = temp_p;
    else
      pred_p -> next = temp_p;
    return 1;
  } else {                                                // Value already in list 
    return 0;
  }
}

/*A function to delete a value*/
int Delete(int value, struct list_node_s** head_p) {      
  struct list_node_s* curr_p = *head_p;
  struct list_node_s* pred_p = NULL;
  while (curr_p != NULL && curr_p -> data < value) {
    pred_p = curr_p;
    curr_p = curr_p -> next;
  }
  
  if (curr_p != NULL && curr_p -> data == value) {
    if (pred_p == NULL) {                                  //Deleting first node in list 
      *head_p = curr_p -> next;
      free(curr_p);
    } else {
      pred_p -> next = curr_p -> next;
      free(curr_p);
    }
    return 1;
  } else {                                                  // Value isn’t in list 
    return 0;
  }
}

