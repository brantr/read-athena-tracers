#include <vector>
#ifndef  READ_ATHENA_TRACERS_H
#define  READ_ATHENA_TRACERS_H

using namespace std;

struct tracer
{
  long id;
  float d;
  float x[3];
  float v[3];
};

long read_athena_tracers(char fname[], vector<tracer> *t);

#endif /*READ_ATHENA_TRACERS_H*/
