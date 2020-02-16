#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>

#include "compute_forces.h"
#include "helper.h"
#include "constants.h"

#if defined(_OPENMP)
#include <omp.h>
#endif


/* compute forces */
void force(mdsys_t *sys) 
{
    double r,ffac;
    double rx,ry,rz;
    int i,j;

#if defined(_OPENMP)
#pragma omp parallel reduction(+:epot)
#endif
    { // begin of parallel region
       double *fx, *fy, *fz;
#if defined(_OPENMP)
       int tid = omp_get_thread_num();
#else
       int tid = 0;
#endif

       fx = sys->fx + (tid*sys->natoms);
       fy = sys->fy + (tid*sys->natoms);
       fz = sys->fz + (tid*sys->natoms);

       /* zero energy and forces */
       sys->epot=0.0;
       azzero(fx,sys->natoms);
       azzero(fy,sys->natoms);
       azzero(fz,sys->natoms);

       for(i=0; i < (sys->natoms); ++i) {
         for(j=0; j < (sys->natoms); ++j) {

            /* particles have no interactions with themselves */
            if (i==j) continue;
            
            /* get distance between particle i and j */
            rx=pbc(sys->rx[i] - sys->rx[j], 0.5*sys->box);
            ry=pbc(sys->ry[i] - sys->ry[j], 0.5*sys->box);
            rz=pbc(sys->rz[i] - sys->rz[j], 0.5*sys->box);
            r = sqrt(rx*rx + ry*ry + rz*rz);
      
            /* compute force and energy if within cutoff */
            if (r < sys->rcut) {
                ffac = -4.0*sys->epsilon*(-12.0*pow(sys->sigma/r,12.0)/r
                                         +6*pow(sys->sigma/r,6.0)/r);
                
                sys->epot += 0.5*4.0*sys->epsilon*(pow(sys->sigma/r,12.0)
                                               -pow(sys->sigma/r,6.0));

                sys->fx[i] += rx/r*ffac;
                sys->fy[i] += ry/r*ffac;
                sys->fz[i] += rz/r*ffac;
            }
         }
       }
   }// end of parallel region
}
