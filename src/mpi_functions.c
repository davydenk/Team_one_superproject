#include "mpi_functions.h"
#include "md_struct.h"
#ifdef USE_MPI
#include <mpi.h>
#endif //USE_MPI
#include "stdlib.h"


void allocate_cs ( mdsys_t * sys ) {
#ifdef USE_MPI
    sys->cx = (double *) malloc( sys->natoms * sizeof(double) );
	  sys->cy = (double *) malloc( sys->natoms * sizeof(double) );
	  sys->cz = (double *) malloc( sys->natoms * sizeof(double) );
#endif
}


void free_cs ( mdsys_t * sys ) {
#ifdef USE_MPI
    free( sys->cx );
    free( sys->cy );
    free( sys->cz );
#endif
}

void get_rank_nps(mdsys_t * sys) {
#ifdef USE_MPI
  int nps,rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &sys->rank);
  MPI_Comm_size(MPI_COMM_WORLD, &sys->nps);
#else
  sys->rank=0;
  sys->nps=1;
#endif  
}


