#include "qm_opts.h"

#include "gromacs/utility/strconvert.h"


void QmOptionsRepr::build(ReprFormatter* strategy)
{
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
    if (ir->bPull)
    {
        PullRepr(ir->pull.get()).build(strategy);
    }

    /* AWH BIASING */
    strategy->pr_named_value("awh", gmx::boolToString(ir->bDoAwh));
    if (ir->bDoAwh)
    {
        AwhRepr(ir->awhParams.get()).build(strategy);
    }

    /* ENFORCED ROTATION */
    strategy->pr_named_value("rotation", gmx::boolToString(ir->bRot));
    if (ir->bRot)
    {
        RotRepr(ir->rot.get()).build(strategy);
    }

    /* INTERACTIVE MD */
    strategy->pr_named_value("interactiveMD", gmx::boolToString(ir->bIMD));
    if (ir->bIMD)
    {
        ImdRepr(ir->imd).build(strategy);
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
        FepRepr(ir->fepvals.get()).build(strategy);
    }
    if (ir->bExpanded)
    {
        ExpandedRepr(ir->expandedvals.get(), ir->fepvals->n_lambda).build(strategy);
    }

    /* NON-equilibrium MD stuff */
    strategy->pr_named_value("cos-acceleration", ir->cos_accel);
    strategy->pr_matrix("deform", ir->deform);

    /* SIMULATED TEMPERING */
    strategy->pr_named_value("simulated-tempering", gmx::boolToString(ir->bSimTemp));
    if (ir->bSimTemp)
    {
        SimTempRepr(ir->simtempvals.get(), ir->fepvals->n_lambda).build(strategy);
    }

    /* ION/WATER SWAPPING FOR COMPUTATIONAL ELECTROPHYSIOLOGY */
    strategy->pr_named_value("swapcoords", enumValueToString(ir->eSwapCoords));
    if (ir->eSwapCoords != SwapType::No)
    {
        SwapRepr(ir->swap).build(strategy);
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

    if (!strategy->bMDPformat)
    {
        strategy->pr_kvtree(*ir->params);
    }

    strategy->close_section();
}

void AwhRepr::build(ReprFormatter* strategy)
{
    strategy->pr_named_value("awh-potential", enumValueToString(awhParams->potential()));
    strategy->pr_named_value("awh-seed", awhParams->seed());
    strategy->pr_named_value("awh-nstout", awhParams->nstout());
    strategy->pr_named_value("awh-nstsample", awhParams->nstSampleCoord());
    strategy->pr_named_value("awh-nsamples-update", awhParams->numSamplesUpdateFreeEnergy());
    strategy->pr_named_value("awh-share-bias-multisim", gmx::boolToString(awhParams->shareBiasMultisim()));
    strategy->pr_named_value("awh-nbias", awhParams->numBias());

    int index = 1;
    for (const auto& awhBiasParams : awhParams->awhBiasParams())
    {
        AwhBiasRepr(index, &awhBiasParams).build(strategy);
        index++;
    }
}

void AwhBiasRepr::build(ReprFormatter* strategy)
{
    strategy->pr_named_value(gmx::formatString("awh%d-error-init", index),
                             awhBiasParams->initialErrorEstimate());
    strategy->pr_named_value(gmx::formatString("awh%d-growth", index),
                             enumValueToString(awhBiasParams->growthType()));
    strategy->pr_named_value(gmx::formatString("awh%d-growth-factor", index),
                             awhBiasParams->growthFactor());
    strategy->pr_named_value(gmx::formatString("awh%d-target", index),
                             enumValueToString(awhBiasParams->targetDistribution()));
    strategy->pr_named_value(gmx::formatString("awh%d-target-beta-scaling", index),
                             awhBiasParams->targetBetaScaling());
    strategy->pr_named_value(gmx::formatString("awh%d-target-cutoff", index),
                             awhBiasParams->targetCutoff());
    strategy->pr_named_value(gmx::formatString("awh%d-target-metric-scaling", index),
                             gmx::boolToString(awhBiasParams->scaleTargetByMetric()));
    strategy->pr_named_value(gmx::formatString("awh%d-target-metric-scaling-limit", index),
                             awhBiasParams->targetMetricScalingLimit());
    strategy->pr_named_value(gmx::formatString("awh%d-user-data", index),
                             gmx::boolToString(awhBiasParams->userPMFEstimate()));
    strategy->pr_named_value(gmx::formatString("awh%d-share-group", index), awhBiasParams->shareGroup());
    strategy->pr_named_value(gmx::formatString("awh%d-equilibrate-histogram", index),
                             gmx::boolToString(awhBiasParams->equilibrateHistogram()));
    strategy->pr_named_value(gmx::formatString("awh%d-ndim", index), awhBiasParams->ndim());

    int dim = 1;
    for (const auto& dimParams : awhBiasParams->dimParams())
    {
        AwhBiasDimRepr(index, dim, &dimParams).build(strategy);
        dim++;
    }
}

void AwhBiasDimRepr::build(ReprFormatter* strategy)
{
    strategy->pr_title_awh(gmx::formatString("awh%d-dim%d", index, dimension));
    strategy->pr_named_value("coord-provider", enumValueToString(awhDimParams->coordinateProvider()));
    strategy->pr_named_value("coord-index", awhDimParams->coordinateIndex() + 1);
    strategy->pr_named_value("start", awhDimParams->origin());
    strategy->pr_named_value("end", awhDimParams->end());
    strategy->pr_named_value("period", awhDimParams->period());
    strategy->pr_named_value("force-constant", awhDimParams->forceConstant());
    strategy->pr_named_value("diffusion", awhDimParams->diffusion());
    strategy->pr_named_value("cover-diameter", awhDimParams->coverDiameter());
    strategy->close_section();
}

void ExpandedRepr::build(ReprFormatter* strategy)
{
    strategy->pr_named_value("nstexpanded", expanded->nstexpanded);
    strategy->pr_named_value("lmc-stats", enumValueToString(expanded->elamstats));
    strategy->pr_named_value("lmc-move", enumValueToString(expanded->elmcmove));
    strategy->pr_named_value("lmc-weights-equil", enumValueToString(expanded->elmceq));
    if (expanded->elmceq == LambdaWeightWillReachEquilibrium::NumAtLambda)
    {
        strategy->pr_named_value("weight-equil-number-all-lambda", expanded->equil_n_at_lam);
    }
    if (expanded->elmceq == LambdaWeightWillReachEquilibrium::Samples)
    {
        strategy->pr_named_value("weight-equil-number-samples", expanded->equil_samples);
    }
    if (expanded->elmceq == LambdaWeightWillReachEquilibrium::Steps)
    {
        strategy->pr_named_value("weight-equil-number-steps", expanded->equil_steps);
    }
    if (expanded->elmceq == LambdaWeightWillReachEquilibrium::WLDelta)
    {
        strategy->pr_named_value("weight-equil-wl-delta", expanded->equil_wl_delta);
    }
    if (expanded->elmceq == LambdaWeightWillReachEquilibrium::Ratio)
    {
        strategy->pr_named_value("weight-equil-count-ratio", expanded->equil_ratio);
    }
    strategy->pr_named_value("lmc-seed", expanded->lmc_seed);
    strategy->pr_named_value("mc-temperature", expanded->mc_temp);
    strategy->pr_named_value("lmc-repeats", expanded->lmc_repeats);
    strategy->pr_named_value("lmc-gibbsdelta", expanded->gibbsdeltalam);
    strategy->pr_named_value("lmc-forced-nstart", expanded->lmc_forced_nstart);
    strategy->pr_named_value("symmetrized-transition-matrix",
                             gmx::boolToString(expanded->bSymmetrizedTMatrix));
    strategy->pr_named_value("nst-transition-matrix", expanded->nstTij);
    strategy->pr_named_value("mininum-var-min", expanded->minvarmin); /*default is reasonable */
    strategy->pr_named_value("weight-c-range", expanded->c_range);    /* default is just C=0 */
    strategy->pr_named_value("wl-scale", expanded->wl_scale);
    strategy->pr_named_value("wl-ratio", expanded->wl_ratio);
    strategy->pr_named_value("init-wl-delta", expanded->init_wl_delta);
    strategy->pr_named_value("wl-oneovert", gmx::boolToString(expanded->bWLoneovert));

    strategy->pr_init_lambda_weights("init-lambda-weights", expanded->init_lambda_weights.data(), n_lambda);
    strategy->pr_named_value("init-weights", gmx::boolToString(expanded->bInit_weights));
}

void FepRepr::build(ReprFormatter* strategy)
{
    strategy->pr_named_value("init-lambda", fep->init_lambda);
    strategy->pr_named_value("init-lambda-state", fep->init_fep_state);
    strategy->pr_named_value("delta-lambda", fep->delta_lambda);
    strategy->pr_named_value("nstdhdl", fep->nstdhdl);

    if (!strategy->bMDPformat)
    {
        strategy->pr_named_value("n-lambdas", fep->n_lambda);
    }
    if (fep->n_lambda > 0)
    {
        strategy->pr_title("separate-dvdl");
        for (auto i : gmx::EnumerationArray<FreeEnergyPerturbationCouplingType, bool>::keys())
        {
            strategy->pr_separate_dvdl(enumValueToString(i), fep->separate_dvdl[i]);
        }
        strategy->close_section();

        strategy->pr_title_all_lambdas("all-lambdas");
        for (auto key : gmx::EnumerationArray<FreeEnergyPerturbationCouplingType, bool>::keys())
        {
            int i = static_cast<int>(key);
            strategy->pr_all_lambda(enumValueToString(key), fep->all_lambda[i].data(), fep->n_lambda);
        }
        strategy->close_section();
    }
    strategy->pr_named_value("calc-lambda-neighbors", fep->lambda_neighbors);
    strategy->pr_named_value("dhdl-print-energy", enumValueToString(fep->edHdLPrintEnergy));
    strategy->pr_named_value("sc-alpha", fep->sc_alpha);
    strategy->pr_named_value("sc-power", fep->sc_power);
    strategy->pr_named_value("sc-r-power", fep->sc_r_power);
    strategy->pr_named_value("sc-sigma", fep->sc_sigma);
    strategy->pr_named_value("sc-sigma-min", fep->sc_sigma_min);
    strategy->pr_named_value("sc-coul", gmx::boolToString(fep->bScCoul));
    strategy->pr_named_value("dh-hist-size", fep->dh_hist_size);
    strategy->pr_named_value("dh-hist-spacing", fep->dh_hist_spacing);
    strategy->pr_named_value("separate-dhdl-file", enumValueToString(fep->separate_dhdl_file));
    strategy->pr_named_value("dhdl-derivatives", enumValueToString(fep->dhdl_derivatives));
    strategy->pr_named_value("sc-function", enumValueToString(fep->softcoreFunction));
    strategy->pr_named_value("sc-gapsys-scale-linpoint-lj", fep->scGapsysScaleLinpointLJ);
    strategy->pr_named_value("sc-gapsys-scale-linpoint-q", fep->scGapsysScaleLinpointQ);
    strategy->pr_named_value("sc-gapsys-sigma-lj", fep->scGapsysSigmaLJ);
}

void ImdRepr::build(ReprFormatter* strategy)
{
    strategy->pr_named_value("IMD-atoms", imd->nat);
    strategy->pr_ivec_block("atom", imd->ind, imd->nat, TRUE);
}

void PullRepr::build(ReprFormatter* strategy)
{
    int g;

    strategy->pr_named_value("pull-cylinder-r", pull->cylinder_r);
    strategy->pr_named_value("pull-constr-tol", pull->constr_tol);
    strategy->pr_named_value("pull-print-COM", gmx::boolToString(pull->bPrintCOM));
    strategy->pr_named_value("pull-print-ref-value", gmx::boolToString(pull->bPrintRefValue));
    strategy->pr_named_value("pull-print-components", gmx::boolToString(pull->bPrintComp));
    strategy->pr_named_value("pull-nstxout", pull->nstxout);
    strategy->pr_named_value("pull-nstfout", pull->nstfout);
    strategy->pr_named_value("pull-pbc-ref-prev-step-com",
                             gmx::boolToString(pull->bSetPbcRefToPrevStepCOM));
    strategy->pr_named_value("pull-xout-average", gmx::boolToString(pull->bXOutAverage));
    strategy->pr_named_value("pull-fout-average", gmx::boolToString(pull->bFOutAverage));
    strategy->pr_named_value("pull-ngroups", pull->ngroup);
    strategy->pr_title_list("pull-groups");
    for (g = 0; g < pull->ngroup; g++)
    {
        PullGroupRepr(g, &(pull->group[g])).build(strategy);
    }
    strategy->close_list();
    strategy->pr_named_value("pull-ncoords", pull->ncoord);
    strategy->pr_title_list("pull-coords");
    for (g = 0; g < pull->ncoord; g++)
    {
        PullCoordRepr(g, &(pull->coord[g])).build(strategy);
    }
    strategy->close_list();
}

void PullGroupRepr::build(ReprFormatter* strategy)
{
    strategy->pr_title_pull_group("pull-group", index);
    strategy->pr_ivec_block("atom", pgrp->ind.data(), pgrp->ind.size(), TRUE);
    strategy->pr_rvec("weight", pgrp->weight.data(), pgrp->weight.size());
    strategy->pr_named_value("pbcatom", pgrp->pbcatom);
    strategy->close_section();
}

void PullCoordRepr::build(ReprFormatter* strategy)
{
    strategy->pr_title_pull_coord("pull-coord", index);
    strategy->pr_named_value("type", enumValueToString(pcrd->eType));
    if (pcrd->eType == PullingAlgorithm::External)
    {
        strategy->pr_named_value("potential-provider", pcrd->externalPotentialProvider.c_str());
    }
    strategy->pr_named_value("geometry", enumValueToString(pcrd->eGeom));
    for (int g = 0; g < pcrd->ngroup; g++)
    {
        strategy->pr_named_value(gmx::formatString("group[%d]", g), pcrd->group[g]);
    }
    strategy->pr_ivec("dim", pcrd->dim, DIM);
    strategy->pr_rvec("origin", pcrd->origin, DIM);
    strategy->pr_rvec("vec", pcrd->vec, DIM);
    strategy->pr_named_value("start", gmx::boolToString(pcrd->bStart));
    strategy->pr_named_value("init", pcrd->init);
    strategy->pr_named_value("rate", pcrd->rate);
    strategy->pr_named_value("k", pcrd->k);
    strategy->pr_named_value("kB", pcrd->kB);
    strategy->close_section();
}

void RotRepr::build(ReprFormatter* strategy)
{
    int g;

    strategy->pr_named_value("rot-nstrout", rot->nstrout);
    strategy->pr_named_value("rot-nstsout", rot->nstsout);
    strategy->pr_named_value("rot-ngroups", rot->grp.size());
    strategy->pr_title_list("rot-groups");
    for (g = 0; g < gmx::ssize(rot->grp); g++)
    {
        RotGroupRepr(g, &(rot->grp[g])).build(strategy);
    }
    strategy->close_list();
}

void RotGroupRepr::build(ReprFormatter* strategy)
{
    strategy->pr_title_rot_group("rot-group", index);
    strategy->pr_named_value("rot-type", enumValueToString(rotg->eType));
    strategy->pr_named_value("rot-massw", gmx::boolToString(rotg->bMassW));
    strategy->pr_ivec_block("atom", rotg->ind, rotg->nat, TRUE);
    strategy->pr_rvecs("x-ref", as_vec_array(rotg->x_ref_original.data()), rotg->x_ref_original.size());
    strategy->pr_rvec("rot-vec", rotg->inputVec, DIM);
    strategy->pr_rvec("rot-pivot", rotg->pivot, DIM);
    strategy->pr_named_value("rot-rate", rotg->rate);
    strategy->pr_named_value("rot-k", rotg->k);
    strategy->pr_named_value("rot-slab-dist", rotg->slab_dist);
    strategy->pr_named_value("rot-min-gauss", rotg->min_gaussian);
    strategy->pr_named_value("rot-eps", rotg->eps);
    strategy->pr_named_value("rot-fit-method", enumValueToString(rotg->eFittype));
    strategy->pr_named_value("rot-potfit-nstep", rotg->PotAngle_nstep);
    strategy->pr_named_value("rot-potfit-step", rotg->PotAngle_step);
    strategy->close_section();
}

void SimTempRepr::build(ReprFormatter* strategy)
{
    strategy->pr_named_value("simulated-tempering-scaling", enumValueToString(simtemp->eSimTempScale));
    strategy->pr_named_value("sim-temp-low", simtemp->simtemp_low);
    strategy->pr_named_value("sim-temp-high", simtemp->simtemp_high);
    strategy->pr_rvec("simulated tempering temperatures", simtemp->temperatures.data(), n_lambda);
}

void SwapRepr::build(ReprFormatter* strategy)
{
    /* Enums for better readability of the code */
    enum
    {
        eCompA = 0,
        eCompB
    };


    strategy->pr_named_value("swap-frequency", swap->nstswap);

    /* The split groups that define the compartments */
    for (int j = 0; j < 2; j++)
    {
        strategy->pr_named_value(gmx::formatString("massw_split%d", j),
                                 gmx::boolToString(swap->massw_split[j]));
        strategy->pr_ivec_block(
                gmx::formatString("split atoms group %d", j), swap->grp[j].ind, swap->grp[j].nat, TRUE);
    }

    /* The solvent group */
    strategy->pr_ivec_block(
            gmx::formatString("solvent group %s",
                              swap->grp[static_cast<int>(SwapGroupSplittingType::Solvent)].molname),
            swap->grp[static_cast<int>(SwapGroupSplittingType::Solvent)].ind,
            swap->grp[static_cast<int>(SwapGroupSplittingType::Solvent)].nat,
            TRUE);

    /* Now print the indices for all the ion groups: */
    for (int ig = static_cast<int>(SwapGroupSplittingType::Count); ig < swap->ngrp; ig++)
    {
        strategy->pr_ivec_block(gmx::formatString("ion group %s", swap->grp[ig].molname),
                                swap->grp[ig].ind,
                                swap->grp[ig].nat,
                                TRUE);
    }

    strategy->pr_named_value("cyl0-r", swap->cyl0r);
    strategy->pr_named_value("cyl0-up", swap->cyl0u);
    strategy->pr_named_value("cyl0-down", swap->cyl0l);
    strategy->pr_named_value("cyl1-r", swap->cyl1r);
    strategy->pr_named_value("cyl1-up", swap->cyl1u);
    strategy->pr_named_value("cyl1-down", swap->cyl1l);
    strategy->pr_named_value("coupl-steps", swap->nAverage);

    /* Print the requested ion counts for both compartments */
    for (int ic = eCompA; ic <= eCompB; ic++)
    {
        for (int ig = static_cast<int>(SwapGroupSplittingType::Count); ig < swap->ngrp; ig++)
        {
            strategy->pr_named_value(gmx::formatString("%s-in-%c", swap->grp[ig].molname, 'A' + ic),
                                     swap->grp[ig].nmolReq[ic]);
        }
    }

    strategy->pr_named_value("threshold", swap->threshold);
    strategy->pr_named_value("bulk-offsetA", swap->bulkOffset[eCompA]);
    strategy->pr_named_value("bulk-offsetB", swap->bulkOffset[eCompB]);
}
