static char help[] = "Test for mesh reordering\n\n";

#include <petscdmplex.h>

typedef struct {
  PetscInt  dim;               /* The topological mesh dimension */
  PetscBool cellSimplex;       /* Flag for simplices */
  PetscBool interpolate;       /* Flag for mesh interpolation */
  PetscBool refinementUniform; /* Uniformly refine the mesh */
  PetscReal refinementLimit;   /* Maximum volume of a refined cell */
  PetscInt  numFields;         /* The number of section fields */
  PetscInt *numComponents;     /* The number of field components */
  PetscInt *numDof;            /* The dof signature for the section */
} AppCtx;

#undef __FUNCT__
#define __FUNCT__ "ProcessOptions"
PetscErrorCode ProcessOptions(AppCtx *options)
{
  PetscInt       len;
  PetscBool      flg;
  PetscErrorCode ierr;

  PetscFunctionBegin;
  options->dim               = 2;
  options->cellSimplex       = PETSC_TRUE;
  options->interpolate       = PETSC_FALSE;
  options->refinementUniform = PETSC_FALSE;
  options->refinementLimit   = 0.0;
  options->numFields         = 1;
  options->numComponents     = NULL;
  options->numDof            = NULL;

  ierr = PetscOptionsBegin(PETSC_COMM_SELF, "", "Meshing Problem Options", "DMPLEX");CHKERRQ(ierr);
  ierr = PetscOptionsInt("-dim", "The topological mesh dimension", "ex10.c", options->dim, &options->dim, NULL);CHKERRQ(ierr);
  ierr = PetscOptionsBool("-cell_simplex", "Flag for simplices", "ex10.c", options->cellSimplex, &options->cellSimplex, NULL);CHKERRQ(ierr);
  ierr = PetscOptionsBool("-interpolate", "Flag for mesh interpolation", "ex10.c", options->interpolate, &options->interpolate, NULL);CHKERRQ(ierr);
  ierr = PetscOptionsBool("-refinement_uniform", "Uniformly refine the mesh", "ex10.c", options->refinementUniform, &options->refinementUniform, NULL);CHKERRQ(ierr);
  ierr = PetscOptionsReal("-refinement_limit", "The maximum volume of a refined cell", "ex10.c", options->refinementLimit, &options->refinementLimit, NULL);CHKERRQ(ierr);
  ierr = PetscOptionsInt("-num_fields", "The number of section fields", "ex10.c", options->numFields, &options->numFields, NULL);CHKERRQ(ierr);
  if (options->numFields) {
    len  = options->numFields;
    ierr = PetscMalloc1(len, &options->numComponents);CHKERRQ(ierr);
    ierr = PetscOptionsIntArray("-num_components", "The number of components per field", "ex10.c", options->numComponents, &len, &flg);CHKERRQ(ierr);
    if (flg && (len != options->numFields)) SETERRQ2(PETSC_COMM_SELF, PETSC_ERR_ARG_WRONG, "Length of components array is %d should be %d", len, options->numFields);
  }
  len  = (options->dim+1) * PetscMax(1, options->numFields);
  ierr = PetscMalloc1(len, &options->numDof);CHKERRQ(ierr);
  ierr = PetscOptionsIntArray("-num_dof", "The dof signature for the section", "ex10.c", options->numDof, &len, &flg);CHKERRQ(ierr);
  if (flg && (len != (options->dim+1) * PetscMax(1, options->numFields))) SETERRQ2(PETSC_COMM_SELF, PETSC_ERR_ARG_WRONG, "Length of dof array is %d should be %d", len, (options->dim+1) * PetscMax(1, options->numFields));
  ierr = PetscOptionsEnd();CHKERRQ(ierr);
  PetscFunctionReturn(0);
};

#undef __FUNCT__
#define __FUNCT__ "CleanupContext"
PetscErrorCode CleanupContext(AppCtx *user)
{
  PetscErrorCode ierr;

  PetscFunctionBegin;
  ierr = PetscFree(user->numComponents);CHKERRQ(ierr);
  ierr = PetscFree(user->numDof);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "TestReordering"
PetscErrorCode TestReordering(DM dm, AppCtx *user)
{
  DM              pdm;
  IS              perm;
  Mat             A, pA;
  PetscInt        bw, pbw;
  MatOrderingType order = MATORDERINGRCM;
  PetscErrorCode  ierr;

  PetscFunctionBegin;
  ierr = DMPlexGetOrdering(dm, order, &perm);CHKERRQ(ierr);
  ierr = DMPlexPermute(dm, perm, &pdm);CHKERRQ(ierr);
  ierr = DMSetFromOptions(pdm);CHKERRQ(ierr);
  ierr = ISDestroy(&perm);CHKERRQ(ierr);
  ierr = DMCreateMatrix(dm, &A);CHKERRQ(ierr);
  ierr = DMCreateMatrix(pdm, &pA);CHKERRQ(ierr);
  ierr = MatComputeBandwidth(A, 0.0, &bw);CHKERRQ(ierr);
  ierr = MatComputeBandwidth(pA, 0.0, &pbw);CHKERRQ(ierr);
  ierr = MatDestroy(&A);CHKERRQ(ierr);
  ierr = MatDestroy(&pA);CHKERRQ(ierr);
  ierr = DMDestroy(&pdm);CHKERRQ(ierr);
  if (pbw > bw) {
    ierr = PetscPrintf(PetscObjectComm((PetscObject) dm), "Ordering method %s increased bandwidth from %d to %d\n", order, bw, pbw);CHKERRQ(ierr);
  } else {
    ierr = PetscPrintf(PetscObjectComm((PetscObject) dm), "Ordering method %s reduced bandwidth from %d to %d\n", order, bw, pbw);CHKERRQ(ierr);
  }
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "main"
int main(int argc, char **argv)
{
  DM             dm;
  PetscSection   s;
  AppCtx         user;
  PetscErrorCode ierr;

  ierr = PetscInitialize(&argc, &argv, NULL, help);CHKERRQ(ierr);
  ierr = ProcessOptions(&user);CHKERRQ(ierr);
  ierr = DMPlexCreateDoublet(PETSC_COMM_WORLD, user.dim, user.cellSimplex, user.interpolate, user.refinementUniform, user.refinementLimit, &dm);CHKERRQ(ierr);
  ierr = DMSetFromOptions(dm);CHKERRQ(ierr);
  ierr = DMPlexCreateSection(dm, user.dim, user.numFields, user.numComponents, user.numDof, 0, NULL, NULL, NULL, NULL, &s);CHKERRQ(ierr);
  ierr = DMSetDefaultSection(dm, s);CHKERRQ(ierr);
  ierr = PetscSectionDestroy(&s);CHKERRQ(ierr);
  ierr = TestReordering(dm, &user);CHKERRQ(ierr);
  ierr = DMDestroy(&dm);CHKERRQ(ierr);
  ierr = CleanupContext(&user);CHKERRQ(ierr);
  ierr = PetscFinalize();
  return 0;
}
