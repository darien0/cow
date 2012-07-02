

%typemap(in) (const char *name)
{
  $1 = PyString_AsString($input);
}

%module cow
%{
#include "cow.h"
void test_trans(double *result, double **args, int **strides, void *udata);
%}

%include "cow.h"
%callback("%(upper)s");
void test_trans(double *result, double **args, int **strides, void *udata);
%nocallback;
