#include "gromacs/tools/dump/builders/qm_opts.h"

void QmOptionsBuilder::build(DumpStrategy* strategy) {
    strategy->pr_title("qm-opts");
    strategy->pr_named_value("ngQM", ir->opts.ngQM);

    /* CONSTRAINT OPTIONS */
    strategy->pr_named_value("constraint-algorithm", enumValueToString(ir->eConstrAlg));
    strategy->pr_named_value("continuation", gmx::boolToString(ir->bContinuation));

    strategy->pr_named_value("Shake-SOR", gmx::boolToString(ir->bShakeSOR));
    strategy->pr_named_value("shake-tol", ir->shake_tol);
    strategy->pr_named_value("lincs-order", ir->nProjOrder);
    strategy->pr_named_value("lincs-iter", ir->nLincsIter);
    strategy->pr_named_value("lincs-warnangle", ir->LincsWarnAngle);

    /* Walls */
    strategy->pr_named_value("nwall", ir->nwall);
    strategy->pr_named_value("wall-type", enumValueToString(ir->wall_type));
    strategy->pr_named_value("wall-r-linpot", ir->wall_r_linpot);
    /* wall-atomtype */
    strategy->pr_named_value("wall-atomtype[0]", ir->wall_atomtype[0]);
    strategy->pr_named_value("wall-atomtype[1]", ir->wall_atomtype[1]);
    /* wall-density */
    strategy->pr_named_value("wall-density[0]", ir->wall_density[0]);
    strategy->pr_named_value("wall-density[1]", ir->wall_density[1]);
    strategy->pr_named_value("wall-ewald-zfac", ir->wall_ewald_zfac);

    /* COM PULLING */
    strategy->pr_named_value("pull", gmx::boolToString(ir->bPull));
    if (ir->bPull) {
        PullBuilder(ir->pull.get()).build(strategy);
    }

    /* AWH BIASING */
    strategy->pr_named_value("awh", gmx::boolToString(ir->bDoAwh));
    if (ir->bDoAwh) {
        AwhBuilder(ir->awhParams.get()).build(strategy);
    }

    /* ENFORCED ROTATION */
    strategy->pr_named_value("rotation", gmx::boolToString(ir->bRot));
    if (ir->bRot) {
        RotBuilder(ir->rot.get()).build(strategy);
    }

    /* INTERACTIVE MD */
    strategy->pr_named_value("interactiveMD", gmx::boolToString(ir->bIMD));
    if (ir->bIMD) {
        ImdBuilder(ir->imd).build(strategy);
    }

    /* NMR refinement stuff */
    strategy->pr_named_value("disre", enumValueToString(ir->eDisre));
    strategy->pr_named_value("disre-weighting", enumValueToString(ir->eDisreWeighting));
    strategy->pr_named_value("disre-mixed", gmx::boolToString(ir->bDisreMixed));
    strategy->pr_named_value("dr-fc", ir->dr_fc);
    strategy->pr_named_value("dr-tau", ir->dr_tau);
    strategy->pr_named_value("nstdisreout", ir->nstdisreout);

    strategy->pr_named_value("orire-fc", ir->orires_fc);
    strategy->pr_named_value("orire-tau", ir->orires_tau);
    strategy->pr_named_value("nstorireout", ir->nstorireout);

    /* FREE ENERGY VARIABLES */
    strategy->pr_named_value("free-energy", enumValueToString(ir->efep));
    if (ir->efep != FreeEnergyPerturbationType::No || ir->bSimTemp)
    {
        FepBuilder(ir->fepvals.get()).build(strategy);
    }
    if (ir->bExpanded)
    {
        ExpandedBuilder(ir->expandedvals.get(), ir->fepvals->n_lambda).build(strategy);
    }

    /* NON-equilibrium MD stuff */
    strategy->pr_named_value("cos-acceleration", ir->cos_accel);
    strategy->pr_matrix("deform", ir->deform);

    /* SIMULATED TEMPERING */
    strategy->pr_named_value("simulated-tempering", gmx::boolToString(ir->bSimTemp));
    if (ir->bSimTemp)
    {
        SimTempBuilder(ir->simtempvals.get(), ir->fepvals->n_lambda).build(strategy);
    }

    /* ION/WATER SWAPPING FOR COMPUTATIONAL ELECTROPHYSIOLOGY */
    strategy->pr_named_value("swapcoords", enumValueToString(ir->eSwapCoords));
    if (ir->eSwapCoords != SwapType::No)
    {
        SwapBuilder(ir->swap).build(strategy);
    }

    /* USER-DEFINED THINGIES */
    strategy->pr_named_value("userint1", ir->userint1);
    strategy->pr_named_value("userint2", ir->userint2);
    strategy->pr_named_value("userint3", ir->userint3);
    strategy->pr_named_value("userint4", ir->userint4);
    strategy->pr_named_value("userreal1", ir->userreal1);
    strategy->pr_named_value("userreal2", ir->userreal2);
    strategy->pr_named_value("userreal3", ir->userreal3);
    strategy->pr_named_value("userreal4", ir->userreal4);

    if (!bMDPformat)
    {
        strategy->pr_kvtree(*ir->params);
    }

    strategy->close_section();
}

void ImdBuilder::build(DumpStrategy* strategy)
{
    strategy->pr_named_value("IMD-atoms", imd->nat);
    strategy->pr_ivec_block("atom", imd->ind, imd->nat);
}

void SimTempBuilder::build(DumpStrategy* strategy)
{
    strategy->pr_named_value("simulated-tempering-scaling", enumValueToString(simtemp->eSimTempScale));
    strategy->pr_named_value("sim-temp-low", simtemp->simtemp_low);
    strategy->pr_named_value("sim-temp-high", simtemp->simtemp_high);
    strategy->pr_rvec("simulated tempering temperatures", simtemp->temperatures.data(), n_lambda);
}