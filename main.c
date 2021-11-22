// Program is a test driver that allows the user to pass the name of a input file
// on the command line. This file is then opened as a binary file for reading and 
// the program sweeps through the file putting the byte offsets of line
// beginnings into an index array.
//
// The program then prints the maximum number of lines in the file and prompts the
// user repeatedly for line(s) number(s) that it then displays to the screen with
// corresponding line numbers.
//
// CIS26B 
//
// Professor Delia-Manuela Gârbacea
//
// 17 May 2012
// Written by Samira C. Oliva
//
// IDE: Version 4.3 (4E109), gcc
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "samira.h" //if run in gcc use this 

#define MAX_LINE 1024
#define ELEMENTS 50   //to realloc for idxAry

//#define TEST_FILE "/Users/samira/Desktop/CompitoN3/cN3/cN3/in.txt"
#define FLUSH while( getchar() != '\n')

//prototypes
void   getAndValidateArgs(int argc, char **argv, FILE **spIn);
int    countLinesInAFile(FILE *spIn);
long   *sweepFileAndBuildByteOffsetIndexArray(FILE *spIn);
void   getAndValidateLineNumbers(FILE *spIn, char *string, long* idxAry, int max);
void   searchLines(FILE *spIn, long* idxAry, int lineCount, int num1, int num2);
void   freeMem(long *idxAry);

int main (int argc, char **argv)
//int main(void)
{
    
    FILE   *spIn;
    int    lineCount;
    long   *idxAry;
    char   line[2000];
    
    //spIn = myFopen(TEST_FILE, "rb"); //for_testing
    
    getAndValidateArgs(argc, argv, &spIn);                                       
    lineCount = countLinesInAFile(spIn);
    printf("There are %d lines in the input file you provided\n\n", lineCount);    
    idxAry = sweepFileAndBuildByteOffsetIndexArray(spIn);

    while(printf("\nEnter a line or range of lines(1 - %d) or \"quit\"\n", 
                 lineCount), gets(line), strcmp(line, "quit"))
    {
        getAndValidateLineNumbers(spIn, line, idxAry, lineCount);
        
    }

    freeMem(idxAry);
    
    return 0;
}    

//-------------------------------------------------------------------
// Function gets arguments from the command line and checks the
// argument count if it's correct: two arguments: the executable
// and the input file name. 
//
// pre:  nothing
//
// post: user should input the exe file name and input file name
//       if the wrong argument count is entered the program will
//       exit
//
void getAndValidateArgs(int argc, char **argv, FILE **spIn)
{
     if(argc != 2 )
     {
         printf("Wrong number of arguments!\n");
         exit(1);
     }
     *spIn = fopen(argv[1],"r");
         
}

//-------------------------------------------------------------------
// Function counts the number of lines in a file
// 
// pre:  spIn
// post: returns and int, the number of lines in a file
int countLinesInAFile(FILE *spIn)
{
    int  lineCount = 0;
    char buffer[MAX_LINE];
    
    while(fgets(buffer, MAX_LINE - 1, spIn)) 
    {
        lineCount++;
    }
    
    return lineCount;
}

//-------------------------------------------------------------------
// Builds and index array to store the byte offsets of the beginning
// of each line in a file, with the each index of the array
// corresponding to each line in a file
//
// pre:  spIn
//
// post: int* and index array containing the byte offsets of the
//       beginning of each line in a given file
//
long *sweepFileAndBuildByteOffsetIndexArray(FILE *spIn)
{
    char buffer[MAX_LINE];
    long *idxAry, *mover;
    int  arySize = 0;
    long byteOffset;

    fseek(spIn, 0, SEEK_SET); 
    idxAry     = (long*)myMalloc(ELEMENTS * sizeof(long));
    mover      = (long*)myMalloc(sizeof(long));
    
    mover = idxAry;
    
    while(fgets(buffer, MAX_LINE - 1, spIn)) 
    {
        byteOffset =  ftell(spIn);
        *mover     =  byteOffset;
        if(++arySize % ELEMENTS == 0)
        {
            idxAry = (long*)myRealloc(idxAry, (arySize + ELEMENTS + 1) * sizeof(long));
            mover  = idxAry + arySize - 1;
        }
        
        mover++;
        
    }//while
    
    //free(mover);  //gives error memleak 8 bytes
    //mover = NULL; 
    
    return idxAry; 
    
}

//-------------------------------------------------------------------
// Function takes in a string and tokenizes it into two strings
// and then sscanf's them into int variables and then checks
// whether they are valid line entries, if so it calls the
// searchLines function otherwise the user is notified of the error
// and must re-enter input
//
// pre:   input line (string), max line count
// 
// post:  input is validated and if valid line(s) are searched
//        else user must re-enter their input
//
void  getAndValidateLineNumbers(FILE *spIn, char *string, long* idxAry, int max)
{
    char *temp1, *temp2, *endPtr;
    int num1, num2;
    
    temp1 = strtok(string, "\040");
    temp2 = strtok(NULL, "\n");
    
    if(temp1)
        num1 = (int)strtol(temp1, &endPtr, 10);
    if(temp2)
        num2 = (int)strtol(temp2, &endPtr, 10);
    
    if( (temp1) && (!temp2))
    {
        num2 = 0;
        
        if((num1 < 1) || (num1 > max))
            printf("Enter a line number that is between 1 and %d\n", max);
        else 
        {
            searchLines(spIn, idxAry,max, num1, num2);
        }
    }
    if(temp1 && temp2)
    {
        if((num1 < 1) || (num2 < 1) || (num1 > max) || (num2 > max) )
            printf("The range must be numbers must be between 1 and %d\n", max);
        else if(num1 >  num2)
            printf("Perhaps you meant to enter the range: %d to %d?\n", num2, num1);
        else 
        {
            searchLines(spIn, idxAry,max, num1, num2);
        }
    }
}


//-------------------------------------------------------------------
// Function searchles a file for a line number or a range of lines
//
// pre:  spIn, line(s) to search, index array, 
//
// post: line(s) is/are printed on the screen
//
void searchLines(FILE *spIn, long* idxAry, int lineCount,  int num1, int num2)
{
    char line[MAX_LINE], *strLine;
    long *ptr;
    int  lineOutput, lineNumer;
    long *ptr2;
    long strLen;
    
    ptr  = idxAry;
    ptr += num1 - 2; //mover ptr to point to that index matching line #
    
    
    if((num1 && !num2))
    {
        fseek(spIn, *ptr , SEEK_SET);  
        ptr2 = ptr;
        ptr2++;
        strLen = (*ptr2) - (*ptr);
        
        fread(line, strLen * sizeof(char), 1, spIn);
        
        if(line)
        {
            strLine = strtok(line, "\n");
            printf("%-5d%s\n", num1, strLine);
        }
        fseek(spIn, -1 * sizeof(char), SEEK_SET);
    }
    else 
    {
        lineOutput = ((num2 - num1) + 1);//number of lines to print 
        lineNumer = num1;
        while (lineOutput)
        {
            fseek(spIn, *ptr , SEEK_SET); 
            ptr2 = ptr;
            ptr2++;
            strLen = (*ptr2) - (*ptr);  

            fread(&line, strLen * sizeof(char), 1, spIn);
            
            if(line)
            {   
                strLine = strtok(line, "\n");
                printf("%-5d%s\n", lineNumer, strLine);
            }
            
            fseek(spIn, -1 * sizeof(char), SEEK_SET);
            
            lineOutput--;
            lineNumer++;
            ptr++;
        }
    }
    
}

//-------------------------------------------------------------------
// Function frees any memory that was dynamically allocated
// 
// pre:   idxAray
//
// post:  idxAray is freed
//
void  freeMem(long *idxAry)
{
    free(idxAry);    
    idxAry = NULL;    
}
