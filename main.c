//Andris St. Cyr
//Panther ID: 4014790

/*I affirm that I wrote this code by myself without help from
 students or sources online.*/

/*This code manipulates the size and direction of a bmp image. It 
 has four functions that enlarges, rotates, vertically flips and 
 horizontally flips the image. In the main, getopt is used to switch
 different cases, where each case contains the flags for enlargement,
 rotation, flips, and output. The program will then output the image
 depending on which flag is being called.*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "bmplib.h"

/*Function used to enlarge the image by a factor of 2. Memory for new 
pixels is dynamically allocated.*/
int enlarge(PIXEL* original, int rows, int cols,
	    PIXEL** new, int* newrows, int* newcols)
{
    int row;
    int col;
    int i;
    int j;
    int scale = 2;
    
    if ((rows <= 0) || (cols <= 0)){
        return -1;
    }
    
    *newrows = scale * rows;
    *newcols = scale * cols;
    *new = (PIXEL*)malloc((*newrows)*(*newcols)*sizeof(PIXEL));
    
    for (row = 0; row < rows; row++){
        for (i = 0; i < scale; i++){
            for (col = 0; col < cols; col++){
                for (j = 0; j < scale; j++){
                    PIXEL* o = original + row*cols + col;
                    PIXEL* n = (*new) + (row*scale+i)*(*newcols)+(col*scale+j);
                    *n = *o;
                }
            }
        }
    }
  return 0;
}

/*Function to rotate the image 90 degrees clockwise. Memory for new 
pixels is dynamically allocated.*/
int rotate(PIXEL* original, int rows, int cols, 
	   PIXEL** new, int* newrows, int* newcols)
{
    int row;
    int col;
    
    *newrows = cols;
    *newcols = rows;
    *new = (PIXEL*)malloc((*newrows)*(*newcols)*sizeof(PIXEL));
    
    for (row = 0; row < rows; row++){
        for (col = 0; col < cols; col++){
            PIXEL* o = original + row*cols + col;
            PIXEL* n = (*new) + (cols - col - 1) * rows + row;
            *n = *o;
        }
    }
  return 0;
}

/*Function to vertically flip the image. Memory for new pixels
 is dynamically allocated.*/
int verticalflip (PIXEL *original, PIXEL **new, int rows, int cols)
{
    int row;
    int col;
    
    if ((rows <= 0) || (cols <= 0)){
        return -1;
    }
    
    *new = (PIXEL*)malloc(rows*cols*sizeof(PIXEL));
    
    for (row = 0; row < rows; row++){
        for (col = 0; col < cols; col++){
            PIXEL* o = original + rows*col + row;
            PIXEL* n = (*new) + rows*col + (rows-1-row);
            *n = *o;
        }
    }
  return 0;
}


/*Function to horizontally flip the image. Memory for new pixels
 is dynamically allocated.*/
int flip (PIXEL *original, PIXEL **new, int rows, int cols)
{
  int row, col;

  if ((rows <= 0) || (cols <= 0)) return -1;

  *new = (PIXEL*)malloc(rows*cols*sizeof(PIXEL));

  for (row=0; row < rows; row++)
    for (col=0; col < cols; col++) {
      PIXEL* o = original + row*cols + col;
      PIXEL* n = (*new) + row*cols + (cols-1-col);
      *n = *o;
    }

  return 0;
}

int main(int argc, char** argv)
{
  
  //Variables are declared.  
  int r, c, nr, nc;
  PIXEL *b, *nb;
  int option;
  int err;
  extern char *optarg;
  extern int optind;
  char *output;
  char *input;
  
  //Flags are initialized.
  int sFlag = 0;
  int rFlag = 0;
  int vFlag = 0;
  int fFlag = 0;
  int oFlag = 0;
  
  //Program will output getopt message if an error occurs.
  static char usage[] = "usage: bmptool [-f | -r | -s | -v] [-o output_file] [input_file]";
  
  while ((option = getopt(argc, argv, "srvfo:")) != -1)
	switch (option) {
	case 's':
		sFlag = 1;
		break;
	case 'r':
		rFlag = 1;
		break;
        case 'v':
                vFlag = 1;
                break;
        case 'f':
                fFlag = 1;
                break;
        case 'o':
                oFlag = 1;
                output = optarg;
                break;
	case '?':
		err = 1;
		break;
        }
       
        
   //Program will use input from file.    
   if (optind < argc){
      input = argv[optind];
   }
        
   //Program will output error message with usage statement.     
   if (err){
      fprintf(stderr, usage, argv[0]);
      exit(1);
   }
        
   //Program calls the readFile function to read the image file.  
   readFile(input, &r, &c, &b);
   
   /*If sFlag, program will call the enlarge function to 
   increase the image size*/
   if (sFlag == 1){
       enlarge(b, r, c, &nb, &nr, &nc);
       r = nr;
       c = nc;
   }
   
   /*If rFlag, program will call the rotate function to
    rotate the image 90 degrees clockwise.*/     
   if (rFlag == 1){
       rotate(b, r, c, &nb, &nr, &nc);
       r = nr;
       c = nc;
   }
   
   /*If vFlag, program will call the function to vertically
    flip the image.*/     
   if (vFlag == 1){
       verticalflip(b, &nb, r, c);
   }
   
   /*If fFlag, program will call the function to horizontally
    flip the image.*/     
   if (fFlag == 1){
      flip(b, &nb, r, c);  
   }
   
   /*If oFlag, program will output the altered image with the
    new file name.*/     
   if (oFlag == 1){
       writeFile(output, r, c, nb);
   }
        
  //Program frees allocated memory.
  free(b);
  free(nb);
  return 0;
}
