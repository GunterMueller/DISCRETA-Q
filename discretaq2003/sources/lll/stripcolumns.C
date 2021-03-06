/*-------------------------------------------------
	call:
		stripcolumns lambda file [sel]
		
	stripcolumns reads a Kramer-Mesner file 
	deletes the columns which contain entries 
	larger than lambda. 
	It also can read a selection line and upper 
	bounds on the variables.
	It writes an input file to diophant.
-------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define zlength 16000
#define MAX_INT 2147483647

char *filename;
char *zp;
int *selectedcolumns;
int selection;
long **matrix;
long lambda;

char zeile[zlength];
FILE *txt;
char detectstring[100];
int RHSflag;
int RHSdirect;


int main(int argc, char *argv[])
{
	int i,j,flag;
	int rows,columns,remainingcolumns;
	int stab_found, sel_found;
	
	long upperbound;

	if ((argc<4)||(argc>5)) {
		printf("Wrong number of parameters in command line input!\n");
		printf("stripcolumns lambda upperbound filename [SEL]\n");
		printf("stripcolumns 1 1 KM.dat 1\n");
		return 1;
	}

	lambda = atoi(argv[1]);
	if (lambda<=0) lambda = MAX_INT;
	filename = argv[3];

	upperbound = atol(argv[2]);

	RHSflag = 0;
	RHSdirect = 0;
	selection = 0;
	if (argc>4) selection = atoi(argv[4]);

/*-------------------------------------------------
	Read the preamble of the file containing the
	linear system and write it immediately 
	to stdout.
-------------------------------------------------*/
	
	txt = fopen(filename, "r");
	sprintf(detectstring,"%% with RHS");
	/* 
	   "with RHS" means that there are two more lines
	   after the matrix with the RHS.
		RHSflag is set to 1 in this case.
	*/
	
	do {
		fgets(zeile,zlength,txt); 
		if (zeile[0]=='%') printf("%s",zeile);
		if (strstr(zeile,detectstring)!=NULL) RHSflag = 1;
	}
	while (zeile[0]=='%');

/*-------------------------------------------------
	Read the matrix size.

	If a 3. parameter is given and positiv then
	the last column of the matrix is the RHS.
	
	RHSdirect is set in this case.
	The number of columns is given without the RHS !
-------------------------------------------------*/
	flag = 0;
	sscanf(zeile,"%d%d%d",&(rows),&(columns),&flag); 
	if (flag!=1) sscanf(zeile,"%d%d",&(rows),&(columns));

	if (flag) RHSdirect = 1;

/*-------------------------------------------------
	Memory allocation of the matrix and the 
	selection vector.
-------------------------------------------------*/
	matrix = (long**)calloc(columns+1,sizeof(long*));
	for(j=0;j<columns+1;j++) {
		matrix[j] = (long*)calloc(rows+1,sizeof(long));
		for (i=0;i<rows+1;i++) matrix[j][i] = 0; 
	}

	selectedcolumns = (int*)calloc(columns,sizeof(int));
	for (i=0;i<columns;i++) selectedcolumns[i] = 1; 
	
/*-------------------------------------------------
	Read the linear system matrix and test if
	the entry is small enough.
-------------------------------------------------*/
	for (j=0;j<rows;j++) {
		for (i=0;i<columns;i++) {
			fscanf(txt,"%ld",&(matrix[i][j]));
			if (matrix[i][j]>lambda && !RHSdirect) selectedcolumns[i] = 0;
		}
		if (RHSdirect) fscanf(txt,"%ld",&(matrix[columns][j]));
	}

/*-------------------------------------------------
	If there is no RHS given, we set it here
	equal to 1 in each component.
-------------------------------------------------*/
	if (!RHSdirect) {
		for (j=0;j<rows;j++) matrix[columns][j] = 1;
	}

/*-------------------------------------------------
	Search for the stabilizer order
-------------------------------------------------*/
	sprintf(detectstring,"STABILIZER-ORDER-K-SETS");
	do {
		zp=fgets(zeile,zlength,txt);  
	} while ((zp!=NULL)&&(strstr(zeile,detectstring)==NULL));

	if (zp!=NULL) {
		for (i=0;i<columns;i++) fscanf(txt,"%ld",&(matrix[i][rows]));
		stab_found = 1;
	} else {
/*		printf("%s not found\n",detectstring); fflush(stdout); */
		stab_found = 0;
	}

/*-------------------------------------------------
	Handle the selection vector.
-------------------------------------------------*/
	if (selection>0) {
		sprintf(detectstring,"SELECTION %d",selection);
		do {
			zp=fgets(zeile,zlength,txt);  
		}
		while ((zp!=NULL)&&(strstr(zeile,detectstring)==NULL));

		if (zp==NULL) {
/*			printf("%s not found\n",detectstring); */
			sel_found = 0;
		} else sel_found = 1;
		
		/* Now get the next line. */		
		if (sel_found) {
			zp=fgets(zeile,zlength,txt); 
			if (zp==NULL) {
/*				printf("Selection vector not found\n"); */
				sel_found = 0;
			} else {
				for (i=0;i<columns;i++) {
					sscanf(zp,"%d",&j);
					zp=&(zp[2]);
					/* columns corresponding to 0 are deleted */
					if (j==0) selectedcolumns[i] = 0;
				}
			}
		}
	}
/*-------------------------------------------------
	Close the input file.
-------------------------------------------------*/
	fclose(txt); 

/*-------------------------------------------------
	Print if the RHS is free or not.
-------------------------------------------------*/
	if (!RHSdirect) {
		printf("%% FREERHS\n%%\n");
	}
	printf("%%\n%% file generated by stripcolumns\n%%\n");

	
/*-------------------------------------------------
	Print the remaining matrix on stdout.
-------------------------------------------------*/
	remainingcolumns = 0;
	for (i=0;i<columns;i++) if (selectedcolumns[i]) remainingcolumns++;

	printf("%d %d",rows,remainingcolumns);
	printf(" 1");
	printf("\n");
	
	for (i=0;i<rows;i++) {
		for (j=0;j<columns;j++) 
			if (selectedcolumns[j]) printf("%ld ",matrix[j][i]);
		printf("%ld ",matrix[columns][i]);
		printf("\n");
		fflush(stdout);
	}

/*-------------------------------------------------
	Print possible upper bounds (only if >1)
-------------------------------------------------*/
	if (upperbound>1) {
		printf("\nBOUNDS\n");
		for (j=0;j<columns;j++) printf("%ld ",upperbound);
		printf("\n");
	}
/*-------------------------------------------------
	Print the remaining stabilizer orders.
-------------------------------------------------*/
	if (stab_found) {
		printf("STABILIZER-ORDER-K-SETS\n");
		for (j=0;j<columns;j++) 
				if (selectedcolumns[j]) printf("%ld ",matrix[j][rows]);
		printf("\n");
	}

/*-------------------------------------------------
	Print a line indicating which columns have been
	deleted.
-------------------------------------------------*/
	printf("SELECTEDCOLUMNS\n");
	printf("%d\n",columns);
	for (j=0;j<columns;j++) printf("%d ",selectedcolumns[j]);
	printf("\n");
	fflush(stdout);

	return 0;
}

/* \endc */
