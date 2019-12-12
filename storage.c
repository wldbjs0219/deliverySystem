#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "storage.h"

#define STORPAGE_FILEATH 		"storage.txt"


/* 
  definition of storage cell structure ----
  members :
  int building : building number of the destination
  int room : room number of the destination
  int cnt : number of packages in the cell
  char passwd[] : password setting (4 characters)
  char *contents : package context (message string)
*/

typedef struct {
	int building;
	int room;
	int cnt;
	char passwd[PASSWD_LEN+1];
	char *context;
} storage_t;


static storage_t** deliverySystem; 			//deliverySystem point to storage_t array
static int storedCnt = 0;					//number of cells occupied
static int systemSize[2] = {0, 0};  		//row/column of the delivery system
static char masterPassword[PASSWD_LEN+1];	//master password




// ------- inner functions ---------------

//print the inside context of a specific cell
//int x, int y : cell to print the context
static void printStorageInside(int x, int y) {
	printf("\n------------------------------------------------------------------------\n");
	printf("------------------------------------------------------------------------\n");
	if (deliverySystem[x][y].cnt > 0)   						//  if deliverySystem[x][y].cnt is over 0(=there is a context in the storage), printf the context of deliverySystem[x][y]
		printf("<<<<<<<<<<<<<<<<<<<<<<<< : %s >>>>>>>>>>>>>>>>>>>>>>>>>>>>\n", deliverySystem[x][y].context);
	else                                  						// if deliverySystem[x][y].cnt is 0(= there is not a context in the storage) , ptintf "empty"
		printf("<<<<<<<<<<<<<<<<<<<<<<<< empty >>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
		
	printf("------------------------------------------------------------------------\n");
	printf("------------------------------------------------------------------------\n\n");
}

//initialize the storage
//int x, int y : cell coordinate to be initialized
static void initStorage(int x, int y) {
	
	int i;   								// the variable for repeat
	
	//set all the member variables as an initial value
	deliverySystem[x][y].cnt=0;    			 // 'cnt=0' means that deliverySystem[x][y] don't include any msg
	deliverySystem[x][y].building=0;		
	deliverySystem[x][y].room=0;			
	deliverySystem[x][y].context=NULL;   	// deliverySystem[x][y].context dosen't point anything
	
	for(i=0;i<PASSWD_LEN+1;i++)   			// each element of password is 0
	{
		deliverySystem[x][y].passwd[i]=0;
	}

}

//get password input and check if it is correct for the cell (x,y)
//int x, int y : cell for password check
//return : 0 - password is matching, -1 - password is not matching
static int inputPasswd(int x, int y) {
	
	char scanned_pw[PASSWD_LEN+1];    				// a password which is scaned from user
	int check;    									// number that checks if two string are same each other
	
	//get password	
	printf(" - password : ");
	scanf("%4s", scanned_pw);   
	
	//check if it is matching- same:0
	check = strcmp(deliverySystem[x][y].passwd, scanned_pw); 
	
	if(strcmp(deliverySystem[x][y].passwd, scanned_pw)==0 || strcmp(scanned_pw, masterPassword) == 0)
	{
		return 0;
	}
	
	else
	{
		return -1;
	}
}


// ------- API function for main.c file ---------------

//backup the delivery system context to the file system
//char* filepath : filepath and name to write
//return : 0 - backup was successfully done, -1 - failed to backup
int str_backupSystem(int x, int y, char* filepath) {
	
	int i,j;   					// variables for repeat
	FILE *fp;     				// file pointer 
	
	fp=fopen(filepath,"a");     // to open the file
	
	if(fp==NULL)        		  // check if the file is opened
	{
		return -1;
	}
	fprintf(fp, "%d %d %d %d %s %s\n", x, y, deliverySystem[x][y].building, deliverySystem[x][y].room, deliverySystem[x][y].passwd, deliverySystem[x][y].context);   
	
	// to clese the file
	fclose(fp);    		
	 
	return 0;
	}

//create delivery system on the double pointer deliverySystem
//char* filepath : filepath and name to read config parameters (row, column, master password, past contexts of the delivery system
//return : 0 - successfully created, -1 - failed to create the system
int str_createSystem(char* filepath) 
{
	//make file NULL
	FILE *fp = NULL;
	char c;
	int i, j;    // variables for repeat
	
	// inputed the storage's row & column
	int row, column;
	
	// open the file by reading
	fp=fopen(filepath, "r");
	
	//check open the file
	if (fp == NULL)
	{
		return -1;
	}
	
	// scan the row & column of storage and masterPassword
	fscanf(fp, "%d %d %s", &systemSize[0], &systemSize[1], masterPassword);
	
	// delivertSystem is allocated memory 
	deliverySystem = (storage_t**)malloc(systemSize[0]*sizeof(storage_t*));
	for(i=0;i<systemSize[0];i++) {
		deliverySystem[i]= (storage_t*)malloc(systemSize[1]*sizeof(storage_t));
	}
	
	// delivertSystem.context is allocated memory
	for(i=0;i<systemSize[0];i++) {
		for(j=0;j<systemSize[1];j++)
			deliverySystem[i][j].context = (char *)malloc(100 * sizeof(char));
	}
	
	// to initialize deliverySystem[i][j].cnt
	for(i=0;i<systemSize[0];i++)
	{
		for(j=0;j<systemSize[1];j++)
		{
			deliverySystem[i][j].cnt=0;
		}
	}
	
	while(fscanf(fp, "%d %d", &row, &column)==2)    //if the row and column are scanned
	{ 
		fscanf(fp, "%d %d %s %s", &deliverySystem[row][column].building, &deliverySystem[row][column].room, deliverySystem[row][column].passwd, deliverySystem[row][column].context);
		deliverySystem[row][column].cnt++;
	}
	
	// to initialize the deliverySystem[row][column].cnt which is not writted on the file
	for(i=0;i<systemSize[0];i++)
	{
		for(j=0;j<systemSize[1];j++)
		{
			if (deliverySystem[row][column].cnt == 0)
			{
				initStorage(row, column);
			}
		}
	}
	// to close the file
	fclose(fp);
	
	return 0;       // if successfully created, return 0
};
		

//free the memory of the deliverySystem 
void str_freeSystem(void) {
	
	int i;       						 //for repeat
	
	//double pointer memory off
	for(i=0;i<systemSize[0];i++)  		
	 	free(deliverySystem[0]);
	free(deliverySystem);	
}



//print the current state of the whole delivery system (which cells are occupied and the destination of the each occupied cells)
void str_printStorageStatus(void) {
	int i, j;    		// the variables for repeat
	//print the current state of the delivery Storage System
	printf("----------------------------- Delivery Storage System Status (%i occupied out of %i )-----------------------------\n\n", storedCnt, systemSize[0]*systemSize[1]);
	
	printf("\t");   
	for (j=0;j<systemSize[1];j++)   		// print the row number
	{
		printf(" %i\t\t",j);       
	}
	printf("\n-----------------------------------------------------------------------------------------------------------------\n");
	
	for (i=0;i<systemSize[0];i++)  					// print the colmn number
	{
		printf("%i|\t",i);
		for (j=0;j<systemSize[1];j++)  				//check whether each strage has msg and print the buliding&room number.
		{	
			if (deliverySystem[i][j].cnt > 0)  		// if msg is in here, print the bulding & room number of storage
			{
				printf("%i,%i\t|\t", deliverySystem[i][j].building, deliverySystem[i][j].room); 
			}
			else                            
			{
				printf(" -  \t|\t");
			}
		}
		printf("\n");
	}
	printf("--------------------------------------- Delivery Storage System Status --------------------------------------------\n\n");
}

  
//check if the input cell (x,y) is valid and whether it is occupied or not
int str_checkStorage(int x, int y) {
	if (x < 0 || x >= systemSize[0])         // check whether the 'x' entered exists within the row
	{
		return -1;
	}
	
	if (y < 0 || y >= systemSize[1])       // check whether the 'y' entered exists within the column
	{
		return -1;
	}
	
	return deliverySystem[x][y].cnt;	 //if (x,y) is valid, return number of packages in the cell
}


//put a package (msg) to the cell
//input parameters                                                                                           
//int x, int y : coordinate of the cell to put the package
//int nBuilding, int nRoom : building and room numbers of the destination
//char msg[] : package context (message string)
//char passwd[] : password string (4 characters)
//return : 0 - successfully put the package, -1 - failed to put
int str_pushToStorage(int x, int y, int nBuilding, int nRoom, char msg[MAX_MSG_SIZE+1], char passwd[PASSWD_LEN+1]) {
	
	int i;   		// the variable for repeat
	int str=0;      //string length
	
	// to put the numebr that is scaned into each member of deliverySystem[x][y]
	deliverySystem[x][y].building = nBuilding ;   
	deliverySystem[x][y].room = nRoom; 
	deliverySystem[x][y].context = msg;
	
	// to put the password that is scaned in to each element of deliverySystem[x][y].passwd
	for(i=0;i<PASSWD_LEN+1;i++)  
	{
		deliverySystem[x][y].passwd[i]=passwd[i];	
	}
	
	// when get msg, increase cnt
	deliverySystem[x][y].cnt ++;    		
	
	// the lenghth of msg[MAX_MSG_SIZE+1]
	str=strlen(msg);    							
	
	// allocate memory by the length of that sentence
	deliverySystem[x][y].context=(char*)malloc(str*sizeof(char));
	
	if(deliverySystem[x][y].context == NULL)  					// if not put the package (deliverySystem[x][y].context's memory is 0), return -1
	{
		return -1;
	}
	
	return 0; 													// if put the package, return 0	
	
}


//extract the package context with password checking
//after password checking, then put the msg string on the screen and re-initialize the storage
//int x, int y : coordinate of the cell to extract
//return : 0 - successfully extracted, -1 = failed to extract
int str_extractStorage(int x, int y) {
	
	int check;    				    // the number which check if password is same with deliverySystem.passwd[x][y]
	
	check = inputPasswd(x, y);  	// put the return value of inputPasswd(x,y) in to check
	
	if(check == 0)       			// check that they are same             
	{
		// put the msg in the screen
		printf("------------------------------------------------------------------------\n");
		printf("   <<<<<<<<<<<<<<<<<<<<<<< : %s >>>>>>>>>>>>>>>>>>>>>>>>>>>>\n", deliverySystem[x][y].context);
		printf("------------------------------------------------------------------------\n");
   
    //re-initialize the storage
	initStorage(x, y); 
	free(deliverySystem[x][y].context);
							
	return 0;
	}
	
	else
	{
		return -1;
	}

}
	
	

//find my package from the storage
//print all the cells (x,y) which has my package
//int nBuilding, int nRoom : my building/room numbers
//return : number of packages that the storage system has
int str_findStorage(int nBuilding, int nRoom) { 

	int i,j;      				
	int same_cnt=0;            	// the number of times 'if'sentence has caught. 
	
	//Compare the values entered and building & room number
	for(i=0;i<systemSize[0];i++)      		// repeat for row   
	{
		for(j=0;j<systemSize[1];j++)  		// repeat for column
		{
			if(deliverySystem[i][j].building == nBuilding && deliverySystem[i][j].room== nRoom)  // when they are same, print row and column
			{
				printf("row: %d, column: %d\n", i,j);
				same_cnt++;	 
			}

		}
	}		
	return same_cnt;
}
