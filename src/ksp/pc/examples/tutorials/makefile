
CFLAGS	        =
FFLAGS          =
CPPFLAGS        =
FPPFLAGS        =
LOCDIR          = src/ksp/pc/examples/tutorials/
EXAMPLESC       = ex1.c ex2.c
EXAMPLESF       =
MANSEC          = PC

include ${PETSC_DIR}/lib/petsc/conf/variables
include ${PETSC_DIR}/lib/petsc/conf/rules

ex1: ex1.o  chkopts
	-${CLINKER}  -o ex1 ex1.o ${PETSC_KSP_LIB}
	${RM} ex1.o

ex2: ex2.o  chkopts
	-${CLINKER}  -o ex2 ex2.o ${PETSC_KSP_LIB}
	${RM} ex2.o

ex3: ex3.o  chkopts
	-${CLINKER}  -o ex3 ex3.o ${PETSC_KSP_LIB}
	${RM} ex3.o

ex4: ex4.o  chkopts
	-${CLINKER}  -o ex4 ex4.o ${PETSC_KSP_LIB}
	${RM} ex4.o

ex5: ex5.o  chkopts
	-${CLINKER}  -o ex5 ex5.o ${PETSC_KSP_LIB}
	${RM} ex5.o

ex6: ex6.o  chkopts
	-${CLINKER}  -o ex6 ex6.o ${PETSC_KSP_LIB}
	${RM} ex6.o

#------------------------------------------------------------------------------------
runex1:
	-@${MPIEXEC} -n 1 ./ex1 -pc_factor_shift_type positive_definite
	-@${RM} -f ex1.tmp
runex4:
	-@${MPIEXEC} -n 1 ./ex4  -fin finan512.mtx -ksp_max_it  100
	-@${RM} -f ex4.tmp

runex6:
	-@${MPIEXEC} -n 1 ./ex6  -fin bcsstm22.mtx -ksp_max_it  10000
	-@${RM} -f ex6.tmp

runex5:
	-@${MPIEXEC} -n 1 ./ex5  cage3.mtx 
	-@${RM} -f ex5.tmp

runex3:
	-@${MPIEXEC} -n 8 ./ex3 -n 100 -ksp_type cg -pc_type bjacobi -sub_pc_type icc -ksp_rtol 1.e-8 > ex3.tmp 2>&1;	  \
	   if (${DIFF} output/ex3_1.out ex3.tmp) then true; \
	   else printf "${PWD}\nPossible problem with ex3_1, diffs above\n=========================================\n"; fi; \
	   ${RM} -f ex3.tmp
runex3_2:
	-@${MPIEXEC} -n 8 ./ex3 -n 100 -ksp_type preonly -pc_type redistribute -redistribute_ksp_type cg -redistribute_pc_type bjacobi -redistribute_sub_pc_type icc -redistribute_ksp_rtol 1.e-8 > ex3.tmp 2>&1;	  \
	   if (${DIFF} output/ex3_2.out ex3.tmp) then true; \
	   else printf "${PWD}\nPossible problem with ex3_2, diffs above\n=========================================\n"; fi; \
	   ${RM} -f ex3.tmp

TESTEXAMPLES_C	 = ex1.PETSc runex1 ex1.rm ex3.PETSc runex3 runex3_2 ex3.rm 
TESTEXAMPLES_C_X = ex2.PETSc ex2.rm

include ${PETSC_DIR}/lib/petsc/conf/test

