

#ifndef SRHDPACK_HEADER_INCLUDED
#define SRHDPACK_HEADER_INCLUDED
#include "cow.h"

#define SRHDPACK_VELOCITY_GAMMA          -42
#define SRHDPACK_VELOCITY_BETA           -43
#define SRHDPACK_VELOCITY_GAMMABETA      -44
#define SRHDPACK_SEPARATION_LAB          -45
#define SRHDPACK_SEPARATION_PROPER       -46
#define SRHDPACK_PROJECTION_NONE         -47
#define SRHDPACK_PROJECTION_TRANSVERSE   -49
#define SRHDPACK_PROJECTION_LONGITUDINAL -48

void srhdpack_shelevequescaling(cow_dfield *vel,
				cow_histogram *hist,
				int velmode,
				int sepmode,
				int projmode,
				int exponent,
				int nbatch,
				int nperbatch,
				int seed);

#endif // SRHDPACK_HEADER_INCLUDED
