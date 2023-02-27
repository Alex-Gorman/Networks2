#include <stdio.h>
#include <stdlib.h>

int main(int args, char *input[]) {

    /* Command line argumnets = 
        1. host name of the receiver 
        2. port number of the receiver 
        3. maximum window sending size
        4. time value in seconds that should be used to trigger re-transmissions
    */

   /* Check for the correct number of arguments */
   if (args != 5) {
        printf("Incorrect number of arguments. Please enter 5 arguments. \n");
        exit(1);
   }

   printf(input[0]);
   







    return 0;
}