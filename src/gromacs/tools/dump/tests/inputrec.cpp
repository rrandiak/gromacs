#include <gtest/gtest.h>
#include <cstdio>

#include "gromacs/tools/dump/builders/inputrec.h"
#include "gromacs/mdtypes/md_enums.h"
#include "gromacs/utility/real.h"

#include "test_base_builder.h"
#include "test_base_pr_func.h"


const std::string INPUTREC_TEXT_OUTPUT = R"(inputrec:
   integrator                     = md
   tinit                          = 0
   dt                             = 0
   nsteps                         = 0
   init-step                      = 0
   simulation-part                = 0
   mts                            = false
   mass-repartition-factor        = 1
   comm-mode                      = Linear
   nstcomm                        = 0
   bd-fric                        = 0
   ld-seed                        = 0
   emtol                          = 0
   emstep                         = 0
   niter                          = 0
   fcstep                         = 0
   nstcgsteep                     = 0
   nbfgscorr                      = 0
   rtpi                           = 0
   nstxout                        = 0
   nstvout                        = 0
   nstfout                        = 0
   nstlog                         = 0
   nstcalcenergy                  = 0
   nstenergy                      = 0
   nstxout-compressed             = 0
   compressed-x-precision         = 0
   cutoff-scheme                  = Verlet
   nstlist                        = 0
   pbc                            = xyz
   periodic-molecules             = false
   verlet-buffer-tolerance        = 0
   verlet-buffer-pressure-tolerance = 0
   rlist                          = 0
   coulombtype                    = Cut-off
   coulomb-modifier               = Potential-shift-Verlet
   rcoulomb-switch                = 0
   rcoulomb                       = 0
   epsilon-r                      = inf
   epsilon-rf                     = inf
   vdw-type                       = Cut-off
   vdw-modifier                   = Potential-shift-Verlet
   rvdw-switch                    = 0
   rvdw                           = 0
   DispCorr                       = No
   table-extension                = 0
   fourierspacing                 = 0
   fourier-nx                     = 0
   fourier-ny                     = 0
   fourier-nz                     = 0
   pme-order                      = 0
   ewald-rtol                     = 0
   ewald-rtol-lj                  = 0
   lj-pme-comb-rule               = Geometric
   ewald-geometry                 = 3d
   epsilon-surface                = 0
   ensemble-temperature-setting   = not available
   tcoupl                         = No
   nsttcouple                     = 0
   nh-chain-length                = 0
   print-nose-hoover-chain-variables = false
   pcoupl                         = No
   refcoord-scaling               = No
   posres-com (3):
      posres-com[0]= 0.00000e+00
      posres-com[1]= 0.00000e+00
      posres-com[2]= 0.00000e+00
   posres-comB (3):
      posres-comB[0]= 0.00000e+00
      posres-comB[1]= 0.00000e+00
      posres-comB[2]= 0.00000e+00
   QMMM                           = false)";

const std::string QM_OPTS_TEXT_OUTPUT = R"(qm-opts:
   ngQM                           = 0
   constraint-algorithm           = Lincs
   continuation                   = false
   Shake-SOR                      = false
   shake-tol                      = 0
   lincs-order                    = 0
   lincs-iter                     = 0
   lincs-warnangle                = 0
   nwall                          = 0
   wall-type                      = 9-3
   wall-r-linpot                  = 0
   wall-atomtype[0]               = 0
   wall-atomtype[1]               = 0
   wall-density[0]                = 0
   wall-density[1]                = 0
   wall-ewald-zfac                = 0
   pull                           = false
   awh                            = false
   rotation                       = false
   interactiveMD                  = false
   disre                          = No
   disre-weighting                = Conservative
   disre-mixed                    = false
   dr-fc                          = 0
   dr-tau                         = 0
   nstdisreout                    = 0
   orire-fc                       = 0
   orire-tau                      = 0
   nstorireout                    = 0
   free-energy                    = no
   cos-acceleration               = 0
   deform (3x3):
      deform[    0]={ 0.00000e+00,  0.00000e+00,  0.00000e+00}
      deform[    1]={ 0.00000e+00,  0.00000e+00,  0.00000e+00}
      deform[    2]={ 0.00000e+00,  0.00000e+00,  0.00000e+00}
   simulated-tempering            = false
   swapcoords                     = no
   userint1                       = 0
   userint2                       = 0
   userint3                       = 0
   userint4                       = 0
   userreal1                      = 0
   userreal2                      = 0
   userreal3                      = 0
   userreal4                      = 0
grpopts:
   nrdf:
   ref-t:
   tau-t:
annealing:
annealing-npoints:
   acc:	
   nfreeze:)";

namespace gmx
{

namespace test
{

TEST_F(PrintFunctionTest, GeneratesCorrectInputRecordDumpTextOld)
{
	t_inputrec ir;
    ir.params = new KeyValueTreeObject();
    t_grpopts opts;
    ir.opts = opts;

    auto func = std::bind(pr_inputrec, std::placeholders::_1, 0, "inputrec", &ir, false);

    // // Verify the output
    runTest(func, INPUTREC_TEXT_OUTPUT + "\n" + QM_OPTS_TEXT_OUTPUT + "\n");
}

TEST_F(DumpBuilderTextTest, GeneratesCorrectInputRecordDumpText)
{
	t_inputrec ir;
    ir.params = new KeyValueTreeObject();
    t_grpopts opts;
    ir.opts = opts;

    InputRecBuilder builder(&ir);

    // Verify the output
    runTest(&builder, "\n" + INPUTREC_TEXT_OUTPUT);
}

} // namespace test

} // namespace gmx
