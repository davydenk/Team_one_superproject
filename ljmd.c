/* 
 * simple lennard-jones potential MD code with velocity verlet.
 * units: Length=Angstrom, Mass=amu; Energy=kcal
 *
 * baseline c version.
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>

#include "compute_forces.h"
#include "helper.h"
#include "kinetic.h"
#include "output.h"
#include "velocity.h"
#include "md_struct.h"
#include "getline.h"
#include "read_input.h"

#ifdef USE_MPI
#include <mpi.h>
#endif //USE_MPI

/* main */
int main() 
{
    int nprint, i, rank, npes;
    char restfile[BLEN], trajfile[BLEN], ergfile[BLEN];
    FILE *fp,*traj,*erg;
    mdsys_t sys;

    //INITIALIZE MPI

    //Move to src/mpi_io.c
	#ifdef USE_MPI
	  MPI_Init();
	  MPI_Comm_size( MPI_COMM_WORLD, &sys->npes );
	  MPI_Comm_rank( MPI_COMM_WORLD, &sys->rank );
	#else
	  sys->rank = 0;
	  sys->npes = 1;
	#endif //USE_MPI

	//READING DATA and if MPI is definite Broadcast
    read_input(&sys, restfile, trajfile, ergfile, &nprint);
    

    /* allocate memory */
    sys.rx=(double *)malloc(sys.natoms*sizeof(double));
    sys.ry=(double *)malloc(sys.natoms*sizeof(double));
    sys.rz=(double *)malloc(sys.natoms*sizeof(double));
    sys.vx=(double *)malloc(sys.natoms*sizeof(double));
    sys.vy=(double *)malloc(sys.natoms*sizeof(double));
    sys.vz=(double *)malloc(sys.natoms*sizeof(double));
    sys.fx=(double *)malloc(sys.natoms*sizeof(double));
    sys.fy=(double *)malloc(sys.natoms*sizeof(double));
    sys.fz=(double *)malloc(sys.natoms*sizeof(double));

    /* read restart */
    fp=fopen(restfile,"r");
    if(fp) {
        for (i=0; i<sys.natoms; ++i) {
            fscanf(fp,"%lf%lf%lf",sys.rx+i, sys.ry+i, sys.rz+i);
        }
        for (i=0; i<sys.natoms; ++i) {
            fscanf(fp,"%lf%lf%lf",sys.vx+i, sys.vy+i, sys.vz+i);
        }
        fclose(fp);
        azzero(sys.fx, sys.natoms);
        azzero(sys.fy, sys.natoms);
        azzero(sys.fz, sys.natoms);
    } else {
        perror("cannot read restart file");
        return 3;
    }
	#ifdef USE_MPI
    broadcast_arrays( sys );
	#endif

    /* initialize forces and energies.*/
    sys.nfi=0;
    force(&sys);
    ekin(&sys);


    erg=fopen(ergfile,"w");
    traj=fopen(trajfile,"w");

    printf("Starting simulation with %d atoms for %d steps.\n",sys.natoms, sys.nsteps);
   printf("     NFI            TEMP            EKIN                 EPOT              ETOT\n");
    output(&sys, erg, traj); 

    /**************************************************/
    /* main MD loop */
    for(sys.nfi=1; sys.nfi <= sys.nsteps; ++sys.nfi) {

        /* write output, if requested */
        if ((sys.nfi % nprint) == 0)
            output(&sys, erg, traj);

        /* propagate system and recompute energies */
        velverlet(&sys);
        ekin(&sys);
    }
    /**************************************************/

    /* clean up: close files, free memory */
    printf("Simulation Done.\n");
    fclose(erg);
    fclose(traj);

    free(sys.rx);
    free(sys.ry);
    free(sys.rz);
    free(sys.vx);
    free(sys.vy);
    free(sys.vz);
    free(sys.fx);
    free(sys.fy);
    free(sys.fz);


	#ifdef USE_MPI
	  MPI_Finalize();
	#endif //USE_MPI

    return 0;
}
