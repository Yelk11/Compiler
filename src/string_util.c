
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *substring(FILE* fp, int position, int length)
{
   char *new_string;
   int character;
 
   new_string = malloc(length+1);
   
   if (new_string == NULL)
   {
      printf("Unable to allocate memory.\n");
      exit(1);
   }
 
   for (character = 0; character < length; character++)
   {
      *(new_string+character) = *(string + position - 1);      
      string++;  
   }
 
   *(new_string+character) = '\0';
 
   return new_string;
}