#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include "constants.h"
#include "velocity_step1.h"
#include "md_struct.h"

int main()
{

   /* initialize mdsys structure */
   mdsys_t sys;
  
   /* precision parameter */
   long double eps = 1e-10;

   /* counting variables */
   sys.natoms = 2;               // natoms
   sys.nfi = 0;
   sys.nsteps = 10000;             // nr MD steps

   /* physical variables */
   sys.mass = 39.948;            // mass in AMU
   sys.epsilon = 0.2379;         // epsilon in kcal/mol
   sys.sigma = 3.405;            // sigma in angstrom
   sys.rcut = 8.5;               // rcut in angstrom
   sys.box = 17.1580;            // box length (in angstrom)
   sys.dt=5.0;                    // MD time step (in fs)

   printf("natoms: %d \n", sys.natoms);

   /* allocate memory */
   sys.rx = (double *)malloc(sys.natoms*sizeof(double));
   sys.ry = (double *)malloc(sys.natoms*sizeof(double));
   sys.rz = (double *)malloc(sys.natoms*sizeof(double));
   sys.vx = (double *)malloc(sys.natoms*sizeof(double));
   sys.vy = (double *)malloc(sys.natoms*sizeof(double));
   sys.vz = (double *)malloc(sys.natoms*sizeof(double));
   sys.fx = (double *)malloc(sys.natoms*sizeof(double));
   sys.fy = (double *)malloc(sys.natoms*sizeof(double));
   sys.fz = (double *)malloc(sys.natoms*sizeof(double));

   printf("* * * TEST VELOCITY STEP1 * * *\n");

//*************************TEST1*************
    /* forces between particle*/

    sys.fx[0] = 0.;
    sys.fy[0] = 0.;
    sys.fz[0] = 0.;
    sys.fx[1] = 0.; 
    sys.fy[1] = 0.;
    sys.fz[1] = 0.;

    vel_step1(&sys);

    printf("Initial force between particles:\n");

    printf("f=(%.8f, %.8f, %.8f),\n",sys.fx[0], sys.fy[0], sys.fz[0]);

    int i, check = 0;

    for (i=0; i < sys.natoms; i ++){
    	if( sys.vx[i]>= eps || sys.vx[i]>= eps || sys.vx[i]>= eps ) ++check;
    }
 
    if(!check) printf("Test-1 on velocity step1 successful!\n");
    else{
      		printf("Test-1 on velocity step1 failed!\n");
      	exit(1);
    }
  
//====================================================================
    /* forces between particle*/
     
    sys.fx[0] = -1.;
    sys.fy[0] = -1.;
    sys.fz[0] = -1.;
    sys.fx[1] = 1.; 
    sys.fy[1] = 1.;
    sys.fz[1] = 1.;
    
    vel_step1(&sys);

    printf("Initial forces between particles:\n");

    printf("f12=(%.8f, %.8f, %.8f),\n", sys.fx[0], sys.fy[0], sys.fz[0]);
    printf("f21=(%.8f, %.8f, %.8f),\n", sys.fx[1], sys.fy[1], sys.fz[1]);

    
    check = 0;
    for (i=0; i < sys.natoms; i ++){
        if( ( fabs(sys.vx[i]) - 0.00002618403924930834) >= eps ) ++check;
        if( ( fabs(sys.vy[i]) - 0.00002618403924930834) >= eps ) ++check;
        if( ( fabs(sys.vz[i]) - 0.00002618403924930834) >= eps ) ++check;
    }

    printf("check value: %d\n", check);

    if(!check) printf("Test-2 on velocity step1 successful!\n");
    else{
      		printf("Test-2 on velocity step1 failed!\n");
      	exit(1);
    }
    
//====================================================================

    free(sys.rx);
    free(sys.ry);
    free(sys.rz);
    free(sys.vx);
    free(sys.vy);
    free(sys.vz);
    free(sys.fx);
    free(sys.fy);
    free(sys.fz);

    return 0;
}
