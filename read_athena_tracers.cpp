#include <stdio.h>
#include <stdlib.h>
#include "read_athena_header.hpp"
#include "read_athena_tracers.hpp"


using namespace std;

long read_athena_tracers(char fname[], vector<tracer> *t)
{
  FILE *fp;
  long n_tracers;       /*number of tracers in the file*/
  float *d;             /*tracer densities*/
  float *x;             /*tracer x positions*/
  float *y;             /*tracer y positions*/
  float *z;             /*tracer z positions*/
  float *vx;            /*tracer x velocities*/
  float *vy;            /*tracer y velocities*/
  float *vz;            /*tracer z velocities*/
  long  *l;             /*tracer ids*/
  long ntd = 0;         /*number of tracers above the density threshold*/
  AthenaHeader *h;


  //buffer for storing tracers into the tracer vector *t
  tracer tin;

  /*open tracer file*/
  if(!(fp = fopen(fname,"r")))
  {
    printf("Error opening %s in load tracers.\n",fname);
    fflush(stdout); 
  }

  /* Read header */
  h = ReadAthenaHeader(fp);

  //ShowAthenaHeader(h);


  /* read the number of tracer in this file */ 
  fread(&n_tracers,1,sizeof(long),fp);

  /* Allocate buffer */
  if(!(d = (float *)malloc(n_tracers*sizeof(float))))
  {
    printf("Error allocating tracer property buf.\n");
    fflush(stdout);
    exit(-1);
  }

  /* Allocate buffer */
  if(!(x = (float *)malloc(n_tracers*sizeof(float))))
  {
    printf("Error allocating tracer property buf.\n");
    fflush(stdout);
    exit(-1);
  }

  /* Allocate buffer */
  if(!(y = (float *)malloc(n_tracers*sizeof(float))))
  {
    printf("Error allocating tracer property y.\n");
    fflush(stdout);
    exit(-1);
  }

  /* Allocate buffer */
  if(!(z = (float *)malloc(n_tracers*sizeof(float))))
  {
    printf("Error allocating tracer property z.\n");
    fflush(stdout);
    exit(-1);
  }

  /* Allocate buffer */
  if(!(vx = (float *)malloc(n_tracers*sizeof(float))))
  {
    printf("Error allocating tracer property z.\n");
    fflush(stdout);
    exit(-1);
  }

    /* Allocate buffer */
    if(!(vy = (float *)malloc(n_tracers*sizeof(float))))
    {
      printf("Error allocating tracer property z.\n");
      fflush(stdout);
      exit(-1);
    }


    /* Allocate buffer */
    if(!(vz = (float *)malloc(n_tracers*sizeof(float))))
    {
      printf("Error allocating tracer property z.\n");
      fflush(stdout);
      exit(-1);
    }


    /* read density */
    fread(d,n_tracers,sizeof(float),fp);

    /* read M1 */
    fread(x,n_tracers,sizeof(float),fp);
    for(long tt=0;tt<n_tracers;tt++)
      vx[tt] = x[tt]/d[tt];
  
    /* read M2 */
    fread(x,n_tracers,sizeof(float),fp);
    for(long tt=0;tt<n_tracers;tt++)
      vy[tt] = x[tt]/d[tt];

    /* read M3 */
    fread(x,n_tracers,sizeof(float),fp);
    for(long tt=0;tt<n_tracers;tt++)
      vz[tt] = x[tt]/d[tt];

#ifndef BAROTROPIC
    /* read E */
    fread(x,n_tracers,sizeof(float),fp);
#endif /* BAROTROPIC */

#ifdef MHD
    /* read B1c */
    fread(x,n_tracers,sizeof(float),fp);

    /* read B2c */
    fread(x,n_tracers,sizeof(float),fp);

    /* read B3c */
    fread(x,n_tracers,sizeof(float),fp);
#endif /*MHD*/

#if (NSCALARS > 0)
    for(k=0;k<NSCALARS;k++)
      fread(x,n_tracers,sizeof(float),fp);
#endif

    /* read x1 */
    fread(x,n_tracers,sizeof(float),fp);
  
    /* read x2 */
    fread(y,n_tracers,sizeof(float),fp);

    /* read x3 */
    fread(z,n_tracers,sizeof(float),fp);  

    /* Allocate buffer */
    if(!(l = (long *)malloc(n_tracers*sizeof(long))))
    {
      printf("Error allocating tracer property buf.\n");
      fflush(stdout);
    }

    /* read id */
    fread(l,n_tracers,sizeof(long),fp);

    /*close tracer file*/
    fclose(fp);


    /*keep only particles with densities above or = threshold*/
    ntd = 0;
    for(long tt=0;tt<n_tracers;tt++)
    {
      tin.id = l[tt];
      tin.d = d[tt];
      tin.x[0] = x[tt];
      tin.x[1] = y[tt];
      tin.x[2] = z[tt];
      tin.v[0] = vx[tt];
      tin.v[1] = vy[tt];
      tin.v[2] = vz[tt];


      //add to tracer list
      (*t).push_back(tin);

      //remember that we've kept a particle
      ntd++;
    }

    /*free buffer memory*/
    free(d);
    free(x);
    free(y);
    free(z);
    free(vx);
    free(vy);
    free(vz);
    free(l);


  /*free header*/
  free(h);


  //return number of tracers > density
  return ntd;

}

