
#requiresdefine   'PETSC_HAVE_FORTRAN'
ALL: lib

CFLAGS   =
FFLAGS   =
SOURCEC  = vinvf.c vscatf.c vpscatf.c vecmpitoseqf.c projectionf.c veciof.c combf.c cmeshf.c
SOURCEF  =
SOURCEH  = 
DIRS     =
LIBBASE  = libpetscvec
LOCDIR   = src/vec/vec/utils/ftn-auto/
include ${PETSC_DIR}/lib/petsc/conf/variables
include ${PETSC_DIR}/lib/petsc/conf/rules
include ${PETSC_DIR}/lib/petsc/conf/test
