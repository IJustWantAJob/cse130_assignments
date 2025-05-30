#include "matrix.h"
#include <unistd.h>
#include <ctype.h>
#include <sys/wait.h>
#include <sys/types.h>

// Return the size of a r x c matrix
size_t sizeMatrix(int r,int c) {
   return r * c * sizeof(int) + sizeof(Matrix);
}

// Allocate memory and initialize a matrix
Matrix* makeMatrix(int r,int c) {
   Matrix* m = malloc(sizeof(Matrix) + r * c * sizeof(int));
   m->r = r;
   m->c = c;
   return m;
}

// Return a pointer to a matrix in the shared region
Matrix* makeMatrixMap(void* zone,int r,int c) {
   Matrix* m = zone;
   m->r = r;
   m->c = c;
   return m;
}

/* Read a single integer from the FILE fd
 * Note: the function uses getc_unlocked to _not_ pay the overhead of locking and
 * unlocking the file for each integer to be read (imagine reading a matrix of 1000x1000,
 * that's 1,000,000 calls to getc and therefore 1,000,000 calls to locking the file. 
 */
int readValue(FILE* fd) {
   int v = 0;
   char ch;
   int neg=1;
   while (((ch = getc_unlocked(fd)) != EOF) && isspace(ch)); // skip WS.      
   while (!isspace(ch)) {
      if (ch=='-')
         neg=-1;
      else
         v = v * 10 + ch - '0';
      ch = getc_unlocked(fd);
   }
   return neg * v;
}

/* Reads a matrix from a file (fd). 
 * The matrix is held in a text format that first conveys the number of rows and columns. 
 * Then, each row is on a line of text and all the values are separated by white spaces. 
 * Namely:
 * r c
 * v0,0 v0,1 .... v0,c-1
 * v1,0 v1,1 .... v1,c-1
 * ....
 * vr-1,0 ....    v_r-1,c-1
*/
Matrix* readMatrix(FILE* fd) {
   int r,c,v;
   int nv = fscanf(fd,"%d %d",&r,&c);
   Matrix* m = makeMatrix(r,c);
   flockfile(fd);
   for(int i=0;i < r;i++)
      for(int j=0;j < c;j++) {
         v = readValue(fd);
         M(m,i,j) = v;
      }
   funlockfile(fd);
   return m;
}

// Free memory allocated for a matrix
void freeMatrix(Matrix* m) {
   free(m);
}

// Print a matrix
void printMatrix(Matrix* m) {
   for(int i=0;i<m->r;i++) {
      for(int j=0;j < m->c; j++)
         printf("%3d ",M(m,i,j));     
      printf("\n");
   }
}

Matrix* multMatrix(Matrix* a, Matrix* b, Matrix* into) {
   for (int i = 0; i < into->r; i++) {
      for (int j = 0; j < into->c; j++) {
         M(into, i, j) = 0;
      }
   }

   for (int i = 0; i < a->r; i++) {
      for (int k = 0; k < a->c; k++) {
         int a_val = M(a,i,k);
         for (int j = 0; j < b->c; j++) {
            M(into, i, j) += a_val * M(b, k, j);
         }
      }
   }
   return into;
}


Matrix* parMultMatrix(int nbW,sem_t* sem,Matrix* a,Matrix* b,Matrix* into) {
   int totalRows = a->r;
   int baseRows = totalRows / nbW;
   int extra = totalRows % nbW;
   int start = 0;

   for (int w = 0; w < nbW; w++) {
      int rows = baseRows + (w < extra ? 1 : 0);
      pid_t pid = fork();
      if (pid < 0) {
         perror("fork");
         exit(1);
      }

      if (pid == 0) {
         for (int i = start; i < start + rows; i++) {
            for (int k = 0; k < a->c; k++) {
               int a_val = M(a, i, k);
               for (int j = 0; j < b->c; j++) {
                  M(into, i, j) += a_val * M(b, k, j);
               }
            }
         }
         sem_post(sem);
         _exit(0);
      }
      start += rows;
   }
   return into;
}

