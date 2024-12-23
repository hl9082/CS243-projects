//file: readcsv.c
//description: The problem here is to read a number of lines of comma-separated census data, and print some simple statistics for the dataset. The program should accept data supplied from standard input, convert certain fields to numbers, compute statistics, and produce a printed report containing the number of zip codes, the total population of all the zip codes, and information about the zipcodes with the fewest and the most people.

//@author Huy Le
//@author hl9082@rit.edu


#include<stdio.h> ///< standard input/output library.
#include<string.h> ///< String operations library.
#include<stdbool.h> ///< Library for boolean operations.
#include <stdarg.h>///< a header that allows functions to accept an indefinite number of arguments
#include <ctype.h>//Types of data to be used.
#include <stdlib.h>//Standard library for other operations.
#define MAX 90000000 ///< The maximum value used to find the minimum number of citizen.
#define MAX_LINE 256 ///< max size of a line.




///Main program to display data.
///@return 0 if successfully executed and 1 if otherwise.
int main(){




char buffer[MAX_LINE];

//skip the 1st line

fgets(buffer,sizeof(buffer),stdin);

long int min=MAX,max=0,s=0,max_zip,min_zip;
int n=0;
while(fgets(buffer,sizeof(buffer),stdin)){char * ptr;
  
  long int zip_code=strtol(buffer,&ptr,10);
  long int total_population=strtol(ptr+1,&ptr,10);
  
  s+=total_population;
  n++;
  
  if(max<total_population){
    max=total_population;
    max_zip=zip_code;
  }
  if(min>total_population){
    min=total_population;
    min_zip=zip_code;
  }
  }

printf("Total population across %d zipcodes is %ld.\n",n,s);

printf("The fewest people live in %ld, population %ld.\n",min_zip,min);

printf("The most people live in %ld, population %ld.\n",max_zip,max);



return 0;
}
