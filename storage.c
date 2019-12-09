#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "storage.h"

#define STORPAGE_FILEATH 		"storage.txt"

#define N 4
#define M 6


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


static storage_t** deliverySystem; 			//deliverySystem
static int storedCnt = 0;					//number of cells occupied
static int systemSize[2] = {0, 0};  		//row/column of the delivery system
static char masterPassword[PASSWD_LEN+1];	//master password




// ------- inner functions ---------------

//print the inside context of a specific cell
//int x, int y : cell to print the context
static void printStorageInside(int x, int y) {
	printf("\n------------------------------------------------------------------------\n");
	printf("------------------------------------------------------------------------\n");
	if (deliverySystem[x][y].cnt > 0)
		printf("<<<<<<<<<<<<<<<<<<<<<<<< : %s >>>>>>>>>>>>>>>>>>>>>>>>>>>>\n", deliverySystem[x][y].context);
	else
		printf("<<<<<<<<<<<<<<<<<<<<<<<< empty >>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
		
	printf("------------------------------------------------------------------------\n");
	printf("------------------------------------------------------------------------\n\n");
}

//initialize the storage
//set all the member variable as an initial value
//and allocate memory to the context pointer
//int x, int y : cell coordinate to be initialized
static void initStorage(int x, int y) {
	//���Ͽ� ����ߴ� �� �������� x,y ����ü �ʱ�ȭ & ���� ������ �޸� �Ҵ� 
}

//get password input and check if it is correct for the cell (x,y)
//int x, int y : cell for password check
//return : 0 - password is matching, -1 - password is not matching
static int inputPasswd(int x, int y) {
	
	char scanned_pw[4];    // a password which is scaned from user
	int check;    		// return of strncmp
		
	printf(" - password : ");
	scanf("%4s", scanned_pw);   //get password
	
	check = strcmp("deliverySystem[x][y].passwd", "scanned_pw");  		//check if it is matching
	
	if( check != 0)
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
	
	FILE *f;

	f=fopen("filepath","r");
	
	if(f==NULL)
	{
		return -1;
	}
		
	fprintf(filepath ,"%d %d %d %d %s %f", x, y, deliverySystem[x][y].building, deliverySystem[x][y].room, deliverySystem[x][y].passwd, deliverySystem[x][y].context);

	fclose(f);
	
	return 0;

}

//create delivery system on the double pointer deliverySystem
//char* filepath : filepath and name to read config parameters (row, column, master password, past contexts of the delivery system
//return : 0 - successfully created, -1 - failed to create the system
int str_createSystem(char* filepath) {
	  //���Ͽ�� l,m ���̰� �ϱ� 1. systemsized[2]={n,m}���� �������� 0���� 
	
	initStorage(x, y);
	FILE *fp;
	
	fp= fopen("filepath","r");
	
	if(fp==NULL)
	{
		return -1;
	}
	
	return 0;
		
}

//free the memory of the deliverySystem 
void str_freeSystem(void) {
	
	free();	
	
}



//print the current state of the whole delivery system (which cells are occupied and the destination of the each occupied cells)
void str_printStorageStatus(void) {
	int i, j;
	printf("----------------------------- Delivery Storage System Status (%i occupied out of %i )-----------------------------\n\n", storedCnt, systemSize[0]*systemSize[1]);
	
	printf("\t");
	for (j=0;j<systemSize[1];j++)
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
	
	int i;

	deliverySystem[x][y].building = nBuilding ;
	deliverySystem[x][y].room = nRoom;
	deliverySystem[x][y].context = msg;
	
	for(i=0;i<PASSWD_LEN+1;i++)
	{
		deliverySystem[x][y].passwd[i]=passwd[i];	
	}
	
	deliverySystem[x][y].cnt ++;    // get msg >> increase cnt
	
	if()
	{
		return -1;
	}
	
	return 0;	
	
}


//extract the package context with password checking
//after password checking, then put the msg string on the screen and re-initialize the storage
//int x, int y : coordinate of the cell to extract
//return : 0 - successfully extracted, -1 = failed to extract
int str_extractStorage(int x, int y) {
	
	int check;
	
	check = inputPasswd(x, y);
	
	if(check != 0)                      
	{
		return -1;
	}
	
	printf("%s\n",deliverySystem[x][y].context);       // put the msg in the screen
	initStorage(x, y); 									//re-initialize thd storage

	return 0;
}
	
	

//find my package from the storage
//print all the cells (x,y) which has my package
//int nBuilding, int nRoom : my building/room numbers
//return : number of packages that the storage system has
int str_findStorage(int nBuilding, int nRoom) { 

	int i,j;      				//variable of for
	int same_cnt=0;            // the number of times 'if'sentence has caught. 
	
	for(i=0;i<N;i++)           //Compare the values entered and building & room number
	{
		for(j=0;j<M;j++)
		{
			if(deliverySystem[i][j].building == nBuilding && deliverySystem[i][j].room== nRoom)
			{
				printf("row: %d, column: %d\n", i,j);
				same_cnt++;	
			}
		}
	}		
	
	return same_cnt;
}
