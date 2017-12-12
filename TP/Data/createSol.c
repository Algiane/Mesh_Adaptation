/**
 * Example of use of the mmg2d library (basic use of mesh adaptation)
 *
 * \author Cécile Dobrzynski (Inria / IMB, Université de Bordeaux)
 * \version 5
 * \copyright GNU Lesser General Public License.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <float.h>

/** To build this application: */
/* 1 you must clone the Mmg repo and build Mmg by hand:
git clone https://github.com/MmgTools/ */

// $ gcc createSol.c -L ~/mmg/build/lib/ -lmmg2d  -I ~/mmg/build/include/
// with ~/mmg/build/lib/ that must be replace with the path toward the Mmg library
// ~/mmg/build/include/ the path toward the Mmg include directory.


/** Include the mmg2d library hader file */
// if the header file is in the "include" directory
// #include "libmmg2d.h"
// if the header file is in "include/mmg/mmg2d"
#include "mmg/mmg2d/libmmg2d.h"


double f(double x, double y) {
  double sol;

  // TO FILL WITH THE WANTED ANALYTIC FUNCTION

  return sol;
}

double scalar_size(double x, double y) {
  double siz;

  // TO FILL WITH THE WANTED SCALAR SIZE.

  return siz;
}

int tensor_size(double x, double y) {
  double siz[6];

  // TO FILL WITH THE WANTED TENSORIAL SIZE.

  return 1;
}


int main(int argc,char *argv[]) {
  MMG5_pMesh      mmgMesh;
  MMG5_pSol       mmgSol;
  int             ier;
  char            *filename, *fileout,*solout;

  fprintf(stdout,"  -- TEST MMG2DLIB \n");

  if ( argc != 4 ) {
    printf(" Usage: %s filein fileout solout\n",argv[0]);
    return(1);
  }

  /* Name and path of the mesh file */
  filename = (char *) calloc(strlen(argv[1]) + 1, sizeof(char));
  if ( filename == NULL ) {
    perror("  ## Memory problem: calloc");
    exit(EXIT_FAILURE);
  }
  strcpy(filename,argv[1]);

  fileout = (char *) calloc(strlen(argv[2]) + 1, sizeof(char));
  if ( fileout == NULL ) {
    perror("  ## Memory problem: calloc");
    exit(EXIT_FAILURE);
  }
  strcpy(fileout,argv[2]);

  solout = (char *) calloc(strlen(argv[3]) + 1, sizeof(char));
  if ( solout == NULL ) {
    perror("  ## Memory problem: calloc");
    exit(EXIT_FAILURE);
  }
  strcpy(solout,argv[3]);

  /** ------------------------------ STEP   I -------------------------- */
  /** 1) Initialisation of mesh and sol structures */
  /* args of InitMesh:
   * MMG5_ARG_start: we start to give the args of a variadic func
   * MMG5_ARG_ppMesh: next arg will be a pointer over a MMG5_pMesh
   * &mmgMesh: pointer toward your MMG5_pMesh (that store your mesh)
   * MMG5_ARG_ppMet: next arg will be a pointer over a MMG5_pSol storing a metric
   * &mmgSol: pointer toward your MMG5_pSol (that store your metric) */

  mmgMesh = NULL;
  mmgSol  = NULL;

  MMG2D_Init_mesh(MMG5_ARG_start,
                  MMG5_ARG_ppMesh,&mmgMesh,MMG5_ARG_ppMet,&mmgSol,
                  MMG5_ARG_end);

 /** 2) Build mesh in MMG5 format */
  /** Two solutions: just use the MMG2D_loadMesh function that will read a .mesh(b)
      file formatted or manually set your mesh using the MMG2D_Set* functions */

  /** with MMG2D_loadMesh function */
  if ( MMG2D_loadMesh(mmgMesh,filename) != 1 )  exit(EXIT_FAILURE);

  /** 3) Build sol in MMG5 format */
  /** Two solutions: just use the MMG2D_loadSol function that will read a .sol(b)
      file formatted or manually set your sol using the MMG2D_Set* functions */

  /** Get mesh size */
  int ne; // Element number
  int ned; // Edge number
  int np; // Point number
  if ( MMG2D_Get_meshSize(mmgMesh,&np,&ne,&ned) !=1 )  exit(EXIT_FAILURE);

  /** Set the solution/metric: sol applied on vertex
      entities, number of vertices=np, in this example, the
      solution/metric is scalar */
  if ( MMG2D_Set_solSize(mmgMesh,mmgSol,MMG5_Vertex,np,MMG5_Scalar) != 1 )
    exit(EXIT_FAILURE);

  double x,y;
  for ( int k=1; k<=np; ++k ) {
    /** Get mesh coordinates */
    if ( MMG2D_Get_vertex(mmgMesh,&x,&y,NULL,NULL,NULL) != 1 )
      exit(EXIT_FAILURE);

    // HERE COMPUTE YOUR ANALYTIC FUNCTION OR METRIC
    double sol = f(x,y);

    /** Give solutions values and positions */
    if ( MMG2D_Set_scalarSol(mmgSol,sol,k) != 1 ) exit(EXIT_FAILURE);
  }

  /** 4) (not mandatory): check if the number of given entities match with mesh size */
  if ( MMG2D_Chk_meshData(mmgMesh,mmgSol) != 1 ) exit(EXIT_FAILURE);

  /* Save msh file that contains the solution and the mesh for vizualisation */
  if ( MMG2D_saveMshMesh(mmgMesh,mmgSol,fileout) != 1 )
    exit(EXIT_FAILURE);

  /* Save solution for mesh adaptation */
  if ( MMG2D_saveSol(mmgMesh,mmgSol,solout) != 1 )
    exit(EXIT_FAILURE);

  /** 3) Free the MMG2D structures */
  MMG2D_Free_all(MMG5_ARG_start,
                 MMG5_ARG_ppMesh,&mmgMesh,MMG5_ARG_ppMet,&mmgSol,
                 MMG5_ARG_end);

  free(fileout);
  fileout = NULL;

  return(0);
}
