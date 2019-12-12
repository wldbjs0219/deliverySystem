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
	if (deliverySystem[x][y].cnt > 0)   	//  if deliverySystem[x][y].cnt is over 0(=there is a context in the storage), printf the context of deliverySystem[x][y]
		printf("<<<<<<<<<<<<<<<<<<<<<<<< : %s >>>>>>>>>>>>>>>>>>>>>>>>>>>>\n", deliverySystem[x][y].context);
	else                                  // if deliverySystem[x][y].cnt is 0(= there is not a context in the storage) , ptintf "empty"
		printf("<<<<<<<<<<<<<<<<<<<<<<<< empty >>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
		
	printf("------------------------------------------------------------------------\n");
	printf("------------------------------------------------------------------------\n\n");
}

//initialize the storage
//and allocate memory to the context pointer
//int x, int y : cell coordinate to be initialized
static void initStorage(int x, int y) {
	
	int i;   // the variable for repeat
	
//set all the member variable as an initial value
	deliverySystem[x][y].cnt=0;    			 // 'cnt=0' means that deliverySystem[x][y] don't include any msg
	deliverySystem[x][y].building=0;		
	deliverySystem[x][y].room=0;			
	deliverySystem[x][y].context=NULL;   	// deliverySystem[x][y]. context dosen't point anything.
	
	for(i=0;i<PASSWD_LEN+1;i++)   			// each element of password is 0
	{
		deliverySystem[x][y].passwd[i]=0;
	}

}

//get password input and check if it is correct for the cell (x,y)
//int x, int y : cell for password check
//return : 0 - password is matching, -1 - password is not matching
static int inputPasswd(int x, int y) {
	
	char scanned_pw[4];    				// a password which is scaned from user
	int check;    						// return of strncmp
	
	//get password	
	printf(" - password : ");
	scanf("%4s", scanned_pw);   
	
	//check if it is matching- same:0
	check = strcmp("deliverySystem[x][y].passwd", "scanned_pw");  		
	
	if( check != 0) 					// if they are different , return -1
	{
		return -1;
	}
		
	return 0;
}


// ------- API function for main.c file ---------------

//backup the delivery system context to the file system
//char* filepath : filepath and name to write
//return : 0 - backup was successfully done, -1 - failed to backup
int str_backupSystem(int x, int y, char* filepath) {
	
	FILE *f;     				// file pointer 
	
	f=fopen(filepath,"a");     // to open the file
	
	if(f==NULL)        		  // check if the file is opened
	{
		return -1;
	}
	
	// to put the information on the txt file
	fprintf(f, "%d %d %d %d %s %f", x, y, deliverySystem[x][y].building, deliverySystem[x][y].room, deliverySystem[x][y].passwd, deliverySystem[x][y].context);   

	// to clese the file
	fclose(f);    		
	 
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
	int i, j;    // variabled for repeat
	
	// inputed the locker's column&row
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
	
	// dlivertSystem is allocated memory 
	deliverySystem = (storage_t**)malloc(systemSize[0]*sizeof(storage_t*));
	for(i=0;i<systemSize[0];i++) {
		deliverySystem[i]= (storage_t*)malloc(systemSize[1]*sizeof(storage_t));
	}
	
	for(i=0;i<systemSize[0];i++) {
		for(j=0;j<systemSize[1];j++)
			deliverySystem[i][j].context = (char *)malloc(100 * sizeof(char));
	}
	
	while(fscanf(fp, "%d %d", &inputrow, &inputcolumn)==2){ 
		fscanf(fp, "%d %d %s %s", &deliverySystem[inputrow][inputcolumn].building, &deliverySystem[inputrow][inputcolumn].room, deliverySystem[inputrow][inputcolumn].passwd, deliverySystem[inputrow][inputcolumn].context);
		printf("%d %d %s %d %d %d %d %s %s", systemSize[0], systemSize[1], 
											  masterPassword, inputrow, inputcolumn, 
											  deliverySystem[inputrow][inputcolumn].building, deliverySystem[inputrow][inputcolumn].room, 
											  deliverySystem[inputrow][inputcolumn].passwd, 
											  deliverySystem[inputrow][inputcolumn].context);
	}
	
	fclose(fp);
	return 0;
};
		

//free the memory of the deliverySystem 
void str_freeSystem(void) {
	
	free();	
	
}



//print the current state of the whole delivery system (which cells are occupied and the destination of the each occupied cells)
void str_printStorageStatus(void) {
	int i, j;
	printf("----------------------------- Delivery Storage System Status (%i occupied out of %i )-----------------------------\n\n", storedCnt, systemSize[0]*systemSize[1]);
	
	printf("\t");
	for (j=0;j<systemSize[1];j++)   // 
	{
		printf(" %i\t\t",j);
	}
	printf("\n-----------------------------------------------------------------------------------------------------------------\n");
	
	for (i=0;i<systemSize[0];i++)
	{
		printf("%i|\t",i);
		for (j=0;j<systemSize[1];j++)
		{
			if (deliverySystem[i][j].cnt > 0)
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
	if (x < 0 || x >= systemSize[0])   
	{
		return -1;
	}
	
	if (y < 0 || y >= systemSize[1])
	{
		return -1;
	}
	
	return deliverySystem[x][y].cnt;	
}


//put a package (msg) to the cell
//input parameters                                                                                           
//int x, int y : coordinate of the cell to put the package
//int nBuilding, int nRoom : building and room numbers of the destination
//char msg[] : package context (message string)
//char passwd[] : password string (4 characters)
//return : 0 - successfully put the package, -1 - failed to put
int str_pushToStorage(int x, int y, int nBuilding, int nRoom, char msg[MAX_MSG_SIZE+1], char passwd[PASSWD_LEN+1]) {
	
	int i;   // the variable for repeat
	
	// to put the numebr that is scaned into each member of deliverySystem[x][y]
	deliverySystem[x][y].building = nBuilding ;   
	deliverySystem[x][y].room = nRoom; 
	deliverySystem[x][y].context = msg;
	
	// to put the password that is scaned in to each element of deliverySystem[x][y].passwd
	for(i=0;i<PASSWD_LEN+1;i++)  
	{
		deliverySystem[x][y].passwd[i]=passwd[i];	
	}
	
	deliverySystem[x][y].cnt ++;    // get msg >> increase cnt
	
	if()  // if not put the package, return -1
	{
		return -1;
	}
	
	return 0; // if put the package, return 0	
	
}


//extract the package context with password checking
//after password checking, then put the msg string on the screen and re-initialize the storage
//int x, int y : coordinate of the cell to extract
//return : 0 - successfully extracted, -1 = failed to extract
int str_extractStorage(int x, int y) {
	
	int check;   // the number which chck if password is same with deliverySystem.passwd[x][y]
	
	check = inputPasswd(x, y);  	//to put the return value of inputPasswd(x,y)
	
	if(check != 0)       			// check that they are same             
	{
		return -1;
	}
	
	// put the msg in the screen
	printf("------------------------------------------------------------------------\n");
	printf("   <<<<<<<<<<<<<<<<<<<<<<< : %s >>>>>>>>>>>>>>>>>>>>>>>>>>>>\n", deliverySystem[x][y].context);
	printf("------------------------------------------------------------------------\n");
   
    //re-initialize thd storage
	initStorage(x, y); 									

	return 0;
}
	
	

//find my package from the storage
//print all the cells (x,y) which has my package
//int nBuilding, int nRoom : my building/room numbers
//return : number of packages that the storage system has
int str_findStorage(int nBuilding, int nRoom) { 

	int i,j;      				
	int same_cnt=0;            	// the number of times 'if'sentence has caught. 
	
	//Compare the values entered and building & room number
	for(i=0;i<N;i++)      		// repeat for row   
	{
		for(j=0;j<M;j++)  		// repeat for column
		{
			if(deliverySystem[i][j].building == nBuilding && deliverySystem[i][j].room== nRoom) // when they are same, print row and column
			{
				printf("row: %d, column: %d\n", i,j);
				same_cnt++;	 
			}

		}
	}		
		return same_cnt;
}
