

#ifndef COW_HEADER_INCLUDED
#define COW_HEADER_INCLUDED
#include <stdlib.h>
// -----------------------------------------------------------------------------
//
// These prototypes constitute the C.O.W. interface
//
// -----------------------------------------------------------------------------
struct cow_dfield; // forward declarations (for opaque data structure)
struct cow_domain;
typedef struct cow_dfield cow_dfield;
typedef struct cow_domain cow_domain;
typedef void (*cow_transform)(double *result, double **args, int **strides,
			      cow_domain *d);

cow_domain *cow_domain_new();
void cow_domain_commit(cow_domain *d);
void cow_domain_del(cow_domain *d);
void cow_domain_setsize(cow_domain *d, int dim, int size);
void cow_domain_setndim(cow_domain *d, int ndim);
void cow_domain_setguard(cow_domain *d, int guard);
void cow_domain_setprocsizes(cow_domain *d, int dim, int size);
int cow_domain_getsize(cow_domain *d, int dim);
int cow_domain_getguard(cow_domain *d);
int cow_domain_getnumlocalzones(cow_domain *d);
void cow_domain_setcollective(cow_domain *d, int mode);
void cow_domain_setchunk(cow_domain *d, int mode);
void cow_domain_setalign(cow_domain *d, int alignthreshold, int diskblocksize);
void cow_domain_readsize(cow_domain *d, const char *fname, const char *dname);

cow_dfield *cow_dfield_new(cow_domain *domain, const char *name);
void cow_dfield_commit(cow_dfield *f);
void cow_dfield_del(cow_dfield *f);
void cow_dfield_addmember(cow_dfield *f, const char *name);
void cow_dfield_setname(cow_dfield *f, const char *name);
void cow_dfield_extract(cow_dfield *f, const int *I0, const int *I1, void *out);
void cow_dfield_replace(cow_dfield *f, const int *I0, const int *I1, void *out);
void cow_dfield_transform(cow_dfield *result, cow_dfield **args, int nargs,
			  cow_transform op);

int cow_dfield_getstride(cow_dfield *d, int dim);
const char *cow_dfield_getname(cow_dfield *f);
const char *cow_dfield_iteratemembers(cow_dfield *f);
const char *cow_dfield_nextmember(cow_dfield *f);
void *cow_dfield_getdata(cow_dfield *f);
void cow_dfield_syncguard(cow_dfield *f);
void cow_dfield_write(cow_dfield *f, const char *fname);
void cow_dfield_read(cow_dfield *f, const char *fname);



#ifdef COW_PRIVATE_DEFS
#if (COW_MPI)
#include <mpi.h>
#endif
#if (COW_HDF5)
#include <hdf5.h>
#endif

void _io_domain_commit(cow_domain *d);
void _io_domain_del(cow_domain *d);

struct cow_domain
{
  double glb_lower[3]; // lower coordinates of global physical domain
  double glb_upper[3]; // upper " "
  double loc_lower[3]; // lower coordinates of local physical domain
  double loc_upper[3]; // upper " "
  int L_nint[3]; // interior zones on the local subgrid
  int L_ntot[3]; // total " ", including guard zones
  int L_strt[3]; // starting index of interior zones on local subgrid
  int G_ntot[3]; // global domain size
  int G_strt[3]; // starting index into global domain
  int n_dims; // number of dimensions: 1, 2, 3
  int n_ghst; // number of guard zones: >= 0
  //  int n_fields; // number of data fields (dynamically adjustable)
  //  int field_iter; // index into data fields array used for iterating over them
  int balanced; // true when all subgrids have the same size
  int committed; // true after cow_domain_commit called, locks out size changes
  //  cow_dfield **fields; // array of pointers to data fields
#if (COW_MPI)
  int comm_rank; // rank with respect to MPI_COMM_WORLD communicator
  int comm_size; // size " "
  int cart_rank; // rank with respect to the cartesian communicator
  int cart_size; // size " "
  int proc_sizes[3]; // number of subgrids along each dimension
  int proc_index[3]; // coordinates of local subgrid in cartesian communicator
  int num_neighbors; // 3, 9, or 27 depending on the domain dimensionality
  int *neighbors; // cartesian ranks of the neighboring processors
  int *send_tags; // tag used to on send calls with respective neighbor
  int *recv_tags; // " "            recv " "
  MPI_Comm mpi_cart; // the cartesian communicator
#endif
#if (COW_HDF5)
  hsize_t L_nint_h5[3];
  hsize_t L_ntot_h5[3];
  hsize_t L_strt_h5[3];
  hsize_t G_ntot_h5[3];
  hsize_t G_strt_h5[3];
  hid_t fapl;
  hid_t dcpl;
  hid_t dxpl;
#endif
} ;

struct cow_dfield
{
  char *name;
  char **members;
  int member_iter;
  int n_members;
  void *data;
  int stride[3];
  int committed;
  cow_domain *domain;
#if (COW_MPI)
  MPI_Datatype *send_type; // chunk of data to be sent to respective neighbor
  MPI_Datatype *recv_type; // " "                 received from " "
#endif
} ;
#endif // COW_PRIVATE_DEFS

#endif // COW_HEADER_INCLUDED
