//Jason Shamayev
//CSE 3320-001
//1001627879

/*  Some example code and prototype - 
    contains many, many problems: should check for return values 
    (especially system calls), handle errors, not use fixed paths,
    handle parameters, put comments, watch out for buffer overflows,
    security problems, use environment variables, etc.
 */

/* Resources:
https://www.tutorialspoint.com/c_standard_library/c_function_fopen.htm used for calculating bytes
https://cboard.cprogramming.com/c-programming/142406-get-last-modified-date-time.html for calculating date

*/

#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>

#define MAXCHAR 1000
#define MAX 200

struct filesort
{
    char name[200];
    size_t size;
    char date[20];
};
int cmpfunc (const void * a, const void * b) {
   int l = ((struct filesort *)a)->size;
   int r = ((struct filesort *)b)->size;
   return (l-r);
}
struct stat attrib;
int main(void) {
    pid_t child;
    DIR * d;
    struct dirent * de;
    int i, c, k;
    char s[256], cmd[256];
    char fileNames[20][256];
    time_t t;
    struct filesort arr_sort[MAX];
    while (1) {

      t = time( NULL );
      time(&t);
      //printf( "Time: %s\n", ctime( &t ));
      printf("-----------------------------------------------\n" );

      getcwd(s, 200);
      printf( "\nCurrent Directory: %s \n", s);
      printf( "Current Time: %s\n", ctime( &t )); // prints time

      d = opendir( "." );
      c = 0;
      printf("Directories:\n");
      while ((de = readdir(d))){
          if ((de->d_type) & DT_DIR) 
            // printf( " ( %d Directory:  %s ) \n", c++, de->d_name);	//prints out all directories
	     printf("  		%d.  %s\n",c++,de->d_name);
      }
      closedir( d );
      printf( "-----------------------------------------\n" );
      d = opendir( "." );
      c = 0;
      printf("Files:\n");
      if(d != NULL){
      while ((de = readdir (d))) 
        {
	i = 0;
	size_t size = sizeof(fileNames)/ sizeof(*fileNames);
	if (((de->d_type) & DT_REG)){
	  strcpy(arr_sort[i].name,de->d_name);
	  FILE *fp;
   		int len; // going to represent bytes (size)
   		fp = fopen(arr_sort[i].name, "r");
   		if( fp == NULL )  {
      		perror ("Error opening file");
      		return(-1);
   		}
   		fseek(fp, 0, SEEK_END);
   		len = ftell(fp);
   		fclose(fp);
		arr_sort[i].size = len;
	
	stat(arr_sort[i].name, &attrib);
	strftime(arr_sort[i].date, MAX, "%d-%m-%y %H:%M:%S",localtime(&(attrib.st_mtime)));
	//printf("%s",arr_sort[i].date); //prints the date and time last modified
		//printf("%ld",arr_sort[i].size); //gets bytes of each file!
	//strcpy(fileNames[i],de->d_name);// saves file names in the array
	//printf("%s",fileNames[i]);
	//printf("		%d. %s \n",c++,fileNames[i]); // prints file names out
	printf("		%d. %s \n",c++,arr_sort[i].name);
        // save filenames in the array 
        i++;

	if((c % 5) == 0){
	printf("\nHit N for Next or P for Previous\n");
	k = getchar();
	switch(k){
	case 'N':
	break;
	case 'P':
	if((i >= 5) || (c >= 5)){
	i = i -5; // goes back in increments of 5 to print previous 5 files
	c = c -5;
	}
	break;
	}
	}	

	}
        }
    closedir (d);
	}
	else{
	perror("Error: ");
	return(-1);
	}
     /* d = opendir( "." );
      c = 0;
      printf("Files:\n");                    
      while ((de = readdir(d))){                    
          if (((de->d_type) & DT_REG))                              
             printf( "		%d. %s \n", c++, de->d_name); // prints out all the files
          if ( ( c % 5) == 0 ) {
             printf( "Hit N for Next\n" );
	     printf("Hit P for Previous\n");
             k = getchar( );
             }
             }
      }
      closedir( d ); */
      printf( "-----------------------------------------\n" );
      
      printf("Operation:\n");
      printf("		D    Display\n		E    Edit\n		R    Run\n		C    Change Directory\n		S    Sort Directory Listing\n		M    Move to Directory\n		X    Remove File\n		A    Rename\n		Q    Quit\n");
      printf( "-----------------------------------------\n" );
  
      c = getchar( ); getchar( );
      switch (c) {
        case 'q': exit(0); /* quit */ 
	case 'd': printf("Display which file?: ");
		  scanf("%s", cmd );
		   FILE *fp;
    		char flen[MAXCHAR];
    		char* filename = cmd;
 
    		fp = fopen(filename, "r");
   		 if (fp == NULL){
        	printf("Could not open file %s",filename);
        	exit(1);
    				}
    		while (fgets(flen, MAXCHAR, fp) != NULL){
       		printf("%s", flen);
    		fclose(fp);
		  /*if((fptr = fopen("program.txt", "x")) == NULL){
			printf("Error opening file\n");
			exit(1); //exits if file pointer returns NULL
			}
		  fscanf(fptr, "%s",cmd);
		  printf("Displaying file:\n %s", cmd);
		  fclose(fptr);
		  return 0; */
			}
		  break;
        case 'e': printf( "Edit what?:" ); // edits a file in pico
                  scanf( "%s", s );
                  strcpy( cmd, "pico ");
                  strcat( cmd, s );
                  system( cmd );
                  break;
        case 'r': printf( "Run what?:" );  // runs a file
                  scanf( "%s", cmd );
		  char *ext = strrchr(cmd, '.');
		  if(strcmp(ext, ".out") == 0) //runs executable file         	(add .exe files)
                  system( cmd );
		  else
		  printf("Not an executable file\n");
                  break;
        case 'c': printf( "Change To?:" ); // change to a directory
                  scanf( "%s", cmd );
                  chdir( cmd );    
                  break;
	case 'x': printf("Remove what file?: ");
		  scanf("%s", cmd );
		  remove( cmd );
		  break;
	case 'm':printf("Move what file?: ");
		 scanf("%s", cmd );
		 break;
	case 'a': printf("Rename what file?: ");
		 scanf("%s", cmd );
		 printf("What is the new name?: ");
		 char str[200];
		 scanf("%s", str);
		 rename(cmd, str );
		 break;
	case 's': printf("Sort by size or date?: ");
		  scanf("%s", cmd);
		  if(strcmp(cmd, "size") == 0){
		  qsort((void*)arr_sort[MAX].size,MAX,sizeof(arr_sort[0].size),cmpfunc);

			}
		  else if(strcmp(cmd,"date") == 0){

		}
		  else{
		  printf("invalid sort");
		  exit(1);
		}
		 break;
   }  

}
}
