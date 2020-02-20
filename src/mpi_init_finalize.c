#ifdef USE_MPI
#include <mpi.h>
#endif //USE_MPI
#include "stdlib.h"

void initialize_mpi(int argc, char** argv){
#ifdef USE_MPI
  MPI_Init(&argc, &argv);
#endif
}

void finalize_mpi () {
  
#ifdef USE_MPI
  MPI_Finalize();
#endif //USE_MPI

  
}
