#include "gromacs/tools/dump/builders/dump_builders_inputrec.h"

void DumpBuilderMts::build(DumpStrategy* strategy) {
    strategy->pr_named_value("mts", gmx::boolToString(useMts));

    // if (useMts) {
    //     for (int mtsIndex = 1; mtsIndex < static_cast<int>(mtsLevels.size()); mtsIndex++) {
    //         const auto&       mtsLevel = mtsLevels[mtsIndex];
    //         const std::string forceKey = gmx::formatString("mts-level%d-forces", mtsIndex + 1);
    //         std::string       forceGroups;

    //         for (int i = 0; i < static_cast<int>(gmx::MtsForceGroups::Count); i++) {
    //             if (mtsLevel.forceGroups[i]) {
    //                 if (!forceGroups.empty()) {
    //                     forceGroups += " ";
    //                 }
    //                 forceGroups += gmx::mtsForceGroupNames[i];
    //             }
    //         }

    //         PS(forceKey.c_str(), forceGroups.c_str());
    //         const std::string factorKey = gmx::formatString("mts-level%d-factor", mtsIndex + 1);
    //         PI(factorKey.c_str(), mtsLevel.stepFactor);
    //     }
    // }
}

void DumpBuilderPull::pr_pull_group(DumpStrategy* strategy, int g, const t_pull_group* pgrp)
{
    strategy->pr_title_i("pull-group", g);
    // strategy->pr_ivec_block(fp, indent, "atom", pgrp->ind.data(), pgrp->ind.size());
    strategy->pr_rvec("weight", pgrp->weight.data(), pgrp->weight.size());
    strategy->pr_named_value("pbcatom", pgrp->pbcatom);
    strategy->close_section();
}

void DumpBuilderPull::pr_pull_coord(DumpStrategy* strategy, int c, const t_pull_coord* pcrd)
{
    strategy->pr_title_i("pull-coord", c);
    strategy->pr_named_value("type", enumValueToString(pcrd->eType));
    if (pcrd->eType == PullingAlgorithm::External)
    {
        strategy->pr_named_value("potential-provider", pcrd->externalPotentialProvider.c_str());
    }
    strategy->pr_named_value("geometry", enumValueToString(pcrd->eGeom));
    // TODO: maybe use some of other pr_* methods?
    for (int g = 0; g < pcrd->ngroup; g++)
    {
        std::string buffer = gmx::formatString("group[%d]", g);
        strategy->pr_named_value(buffer.c_str(), pcrd->group[g]);
    }
    const int list[] = {1, 2, 3};
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

void DumpBuilderPull::build(DumpStrategy* strategy)
{
    int g;

    strategy->pr_named_value("pull-cylinder-r", pull->cylinder_r);
    strategy->pr_named_value("pull-constr-tol", pull->constr_tol);
    strategy->pr_named_value("pull-print-COM", gmx::boolToString(pull->bPrintCOM));
    strategy->pr_named_value("pull-print-ref-value", gmx::boolToString(pull->bPrintRefValue));
    strategy->pr_named_value("pull-print-components", gmx::boolToString(pull->bPrintComp));
    strategy->pr_named_value("pull-nstxout", pull->nstxout);
    strategy->pr_named_value("pull-nstfout", pull->nstfout);
    strategy->pr_named_value("pull-pbc-ref-prev-step-com", gmx::boolToString(pull->bSetPbcRefToPrevStepCOM));
    strategy->pr_named_value("pull-xout-average", gmx::boolToString(pull->bXOutAverage));
    strategy->pr_named_value("pull-fout-average", gmx::boolToString(pull->bFOutAverage));
    strategy->pr_named_value("pull-ngroups", pull->ngroup);
    for (g = 0; g < pull->ngroup; g++)
    {
        this->pr_pull_group(strategy, g, &(pull->group[g]));
    }
    strategy->pr_named_value("pull-ncoords", pull->ncoord);
    for (g = 0; g < pull->ncoord; g++)
    {
        this->pr_pull_coord(strategy, g, &(pull->coord[g]));
    }
}

void DumpBuilderAwh::pr_awh_bias_dim(DumpStrategy* strategy, const gmx::AwhDimParams& awhDimParams, const char* prefix)
{
    strategy->pr_title(prefix);
    strategy->pr_named_value("coord-provider", enumValueToString(awhDimParams.coordinateProvider()));
    strategy->pr_named_value("coord-index", awhDimParams.coordinateIndex() + 1);
    strategy->pr_named_value("start", awhDimParams.origin());
    strategy->pr_named_value("end", awhDimParams.end());
    strategy->pr_named_value("period", awhDimParams.period());
    strategy->pr_named_value("force-constant", awhDimParams.forceConstant());
    strategy->pr_named_value("diffusion", awhDimParams.diffusion());
    strategy->pr_named_value("cover-diameter", awhDimParams.coverDiameter());
    strategy->close_section();
}

void DumpBuilderAwh::pr_awh_bias(DumpStrategy* strategy, const gmx::AwhBiasParams& awhBiasParams, const char* prefix)
{
    char opt[STRLEN];

    // TODO: create pr_* method, or reuse pr_title with union type
    sprintf(opt, "%s-error-init", prefix);
    strategy->pr_named_value(opt, awhBiasParams.initialErrorEstimate());
    sprintf(opt, "%s-growth", prefix);
    strategy->pr_named_value(opt, enumValueToString(awhBiasParams.growthType()));
    sprintf(opt, "%s-growth-factor", prefix);
    strategy->pr_named_value(opt, awhBiasParams.growthFactor());
    sprintf(opt, "%s-target", prefix);
    strategy->pr_named_value(opt, enumValueToString(awhBiasParams.targetDistribution()));
    sprintf(opt, "%s-target-beta-scaling", prefix);
    strategy->pr_named_value(opt, awhBiasParams.targetBetaScaling());
    sprintf(opt, "%s-target-cutoff", prefix);
    strategy->pr_named_value(opt, awhBiasParams.targetCutoff());
    sprintf(opt, "%s-target-metric-scaling", prefix);
    strategy->pr_named_value(opt, gmx::boolToString(awhBiasParams.scaleTargetByMetric()));
    sprintf(opt, "%s-target-metric-scaling-limit", prefix);
    strategy->pr_named_value(opt, awhBiasParams.targetMetricScalingLimit());
    sprintf(opt, "%s-user-data", prefix);
    strategy->pr_named_value(opt, gmx::boolToString(awhBiasParams.userPMFEstimate()));
    sprintf(opt, "%s-share-group", prefix);
    strategy->pr_named_value(opt, awhBiasParams.shareGroup());
    sprintf(opt, "%s-equilibrate-histogram", prefix);
    strategy->pr_named_value(opt, gmx::boolToString(awhBiasParams.equilibrateHistogram()));
    sprintf(opt, "%s-ndim", prefix);
    strategy->pr_named_value(opt, awhBiasParams.ndim());

    int d = 0;
    for (const auto& dimParam : awhBiasParams.dimParams())
    {
        char prefixdim[STRLEN];
        sprintf(prefixdim, "%s-dim%d", prefix, d + 1);
        this->pr_awh_bias_dim(strategy, dimParam, prefixdim);
        d++;
    }
}

void DumpBuilderAwh::build(DumpStrategy* strategy)
{
    strategy->pr_named_value("awh-potential", enumValueToString(awhParams->potential()));
    strategy->pr_named_value("awh-seed", awhParams->seed());
    strategy->pr_named_value("awh-nstout", awhParams->nstout());
    strategy->pr_named_value("awh-nstsample", awhParams->nstSampleCoord());
    strategy->pr_named_value("awh-nsamples-update", awhParams->numSamplesUpdateFreeEnergy());
    strategy->pr_named_value("awh-share-bias-multisim", gmx::boolToString(awhParams->shareBiasMultisim()));
    strategy->pr_named_value("awh-nbias", awhParams->numBias());

    int k = 0;
    for (const auto& awhBiasParam : awhParams->awhBiasParams())
    {
        auto prefix = gmx::formatString("awh%d", k + 1);
        this->pr_awh_bias(strategy, awhBiasParam, prefix.c_str());
        k++;
    }
}

void DumpBuilderRot::pr_rotgrp(DumpStrategy* strategy, int g, const t_rotgrp* rotg)
{
    strategy->pr_title_i("rot-group", g);
    strategy->pr_named_value("rot-type", enumValueToString(rotg->eType));
    strategy->pr_named_value("rot-massw", gmx::boolToString(rotg->bMassW));
    strategy->pr_ivec_block("atom", rotg->ind, rotg->nat);
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

void DumpBuilderRot::build(DumpStrategy* strategy)
{
    int g;

    strategy->pr_named_value("rot-nstrout", rot->nstrout);
    strategy->pr_named_value("rot-nstsout", rot->nstsout);
    strategy->pr_named_value("rot-ngroups", rot->grp.size());
    for (g = 0; g < gmx::ssize(rot->grp); g++)
    {
        this->pr_rotgrp(strategy, g, &rot->grp[g]);
    }
}

void DumpBuilderImd::build(DumpStrategy* strategy)
{
    strategy->pr_named_value("IMD-atoms", imd->nat);
    strategy->pr_ivec_block("atom", imd->ind, imd->nat);
}

void DumpBuilderFep::build(DumpStrategy* strategy)
{
    int j;

    strategy->pr_named_value("init-lambda", fep->init_lambda);
    strategy->pr_named_value("init-lambda-state", fep->init_fep_state);
    strategy->pr_named_value("delta-lambda", fep->delta_lambda);
    strategy->pr_named_value("nstdhdl", fep->nstdhdl);

    if (!bMDPformat)
    {
        strategy->pr_named_value("n-lambdas", fep->n_lambda);
    }
    if (fep->n_lambda > 0)
    {
        // pr_indent(fp, indent);
        // fprintf(fp, "separate-dvdl%s\n" ? " = " : ":");
        // for (auto i : gmx::EnumerationArray<FreeEnergyPerturbationCouplingType, bool>::keys())
        // {
        //     fprintf(fp, "%18s = ", enumValueToString(i));
        //     if (fep->separate_dvdl[i])
        //     {
        //         fprintf(fp, "  TRUE");
        //     }
        //     else
        //     {
        //         fprintf(fp, "  FALSE");
        //     }
        //     fprintf(fp, "\n");
        // }
        // fprintf(fp, "all-lambdas%s\n" ? " = " : ":");
        // for (auto key : gmx::EnumerationArray<FreeEnergyPerturbationCouplingType, bool>::keys())
        // {
        //     fprintf(fp, "%18s = ", enumValueToString(key));
        //     int i = static_cast<int>(key);
        //     for (j = 0; j < fep->n_lambda; j++)
        //     {
        //         fprintf(fp, "  %10g", fep->all_lambda[i][j]);
        //     }
        //     fprintf(fp, "\n");
        // }
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
};

void DumpBuilderExpanded::build(DumpStrategy* strategy)
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
    strategy->pr_named_value("symmetrized-transition-matrix", gmx::boolToString(expanded->bSymmetrizedTMatrix));
    strategy->pr_named_value("nst-transition-matrix", expanded->nstTij);
    strategy->pr_named_value("mininum-var-min", expanded->minvarmin); /*default is reasonable */
    strategy->pr_named_value("weight-c-range", expanded->c_range);    /* default is just C=0 */
    strategy->pr_named_value("wl-scale", expanded->wl_scale);
    strategy->pr_named_value("wl-ratio", expanded->wl_ratio);
    strategy->pr_named_value("init-wl-delta", expanded->init_wl_delta);
    strategy->pr_named_value("wl-oneovert", gmx::boolToString(expanded->bWLoneovert));

    strategy->pr_rvec("init-lambda-weights", expanded->init_lambda_weights.data(), n_lambda);
    strategy->pr_named_value("init-weights", gmx::boolToString(expanded->bInit_weights));
}

void DumpBuilderSimtemp::build(DumpStrategy* strategy)
{
    strategy->pr_named_value("simulated-tempering-scaling", enumValueToString(simtemp->eSimTempScale));
    strategy->pr_named_value("sim-temp-low", simtemp->simtemp_low);
    strategy->pr_named_value("sim-temp-high", simtemp->simtemp_high);
    strategy->pr_rvec("simulated tempering temperatures", simtemp->temperatures.data(), n_lambda);
}

void DumpBuilderSwap::build(DumpStrategy* strategy)
{
    char str[STRLEN];

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
        snprintf(str, STRLEN, "massw_split%d", j);
        strategy->pr_named_value(str, gmx::boolToString(swap->massw_split[j]));
        snprintf(str, STRLEN, "split atoms group %d", j);
        strategy->pr_ivec_block(str, swap->grp[j].ind, swap->grp[j].nat);
    }

    /* The solvent group */
    snprintf(str,
             STRLEN,
             "solvent group %s",
             swap->grp[static_cast<int>(SwapGroupSplittingType::Solvent)].molname);
    strategy->pr_ivec_block(
        str,
        swap->grp[static_cast<int>(SwapGroupSplittingType::Solvent)].ind,
        swap->grp[static_cast<int>(SwapGroupSplittingType::Solvent)].nat
    );

    /* Now print the indices for all the ion groups: */
    for (int ig = static_cast<int>(SwapGroupSplittingType::Count); ig < swap->ngrp; ig++)
    {
        snprintf(str, STRLEN, "ion group %s", swap->grp[ig].molname);
        strategy->pr_ivec_block(str, swap->grp[ig].ind, swap->grp[ig].nat);
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
            snprintf(str, STRLEN, "%s-in-%c", swap->grp[ig].molname, 'A' + ic);
            strategy->pr_named_value(str, swap->grp[ig].nmolReq[ic]);
        }
    }

    strategy->pr_named_value("threshold", swap->threshold);
    strategy->pr_named_value("bulk-offsetA", swap->bulkOffset[eCompA]);
    strategy->pr_named_value("bulk-offsetB", swap->bulkOffset[eCompB]);
}

void DumpBuilderQmOpts::build(DumpStrategy* strategy) {
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
        DumpBuilderPull(ir->pull.get()).build(strategy);
    }

    /* AWH BIASING */
    strategy->pr_named_value("awh", gmx::boolToString(ir->bDoAwh));
    if (ir->bDoAwh) {
        DumpBuilderAwh(ir->awhParams.get()).build(strategy);
    }

    /* ENFORCED ROTATION */
    strategy->pr_named_value("rotation", gmx::boolToString(ir->bRot));
    if (ir->bRot) {
        DumpBuilderRot(ir->rot.get()).build(strategy);
    }

    /* INTERACTIVE MD */
    strategy->pr_named_value("interactiveMD", gmx::boolToString(ir->bIMD));
    if (ir->bIMD) {
        DumpBuilderImd(ir->imd).build(strategy);
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
        DumpBuilderFep(ir->fepvals.get()).build(strategy);
    }
    if (ir->bExpanded)
    {
        DumpBuilderExpanded(ir->expandedvals.get(), ir->fepvals->n_lambda).build(strategy);
    }

    /* NON-equilibrium MD stuff */
    strategy->pr_named_value("cos-acceleration", ir->cos_accel);
    strategy->pr_matrix("deform", ir->deform);

    /* SIMULATED TEMPERING */
    strategy->pr_named_value("simulated-tempering", gmx::boolToString(ir->bSimTemp));
    if (ir->bSimTemp)
    {
        DumpBuilderSimtemp(ir->simtempvals.get(), ir->fepvals->n_lambda).build(strategy);
    }

    /* ION/WATER SWAPPING FOR COMPUTATIONAL ELECTROPHYSIOLOGY */
    strategy->pr_named_value("swapcoords", enumValueToString(ir->eSwapCoords));
    if (ir->eSwapCoords != SwapType::No)
    {
        DumpBuilderSwap(ir->swap).build(strategy);
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
        // DumpBuilderKVTree(*ir->params).build(strategy);
        strategy->pr_kvtree(*ir->params);
    }

    strategy->close_section();
}

void DumpBuilderGrpOpts::build(DumpStrategy* strategy) {
    // TODO: ensure prev formatting
    // TODO: print enum vector
    int i, m, j;

    if (!bMDPformat)
    {
        strategy->pr_title("grpopts");
    }

    // fprintf(out, "nrdf%s" ? " = " : ":");
    // for (i = 0; (i < opts->ngtc); i++)
    // {
    //     fprintf(out, "  %10g", opts->nrdf[i]);
    // }
    // fprintf(out, "\n");
    strategy->pr_rvec_row("nrdf", opts->nrdf, opts->ngtc);

    // pr_indent(out, indent);
    // fprintf(out, "ref-t%s" ? " = " : ":");
    // for (i = 0; (i < opts->ngtc); i++)
    // {
    //     fprintf(out, "  %10g", opts->ref_t[i]);
    // }
    // fprintf(out, "\n");
    strategy->pr_rvec_row("ref-t", opts->ref_t, opts->ngtc);

    // pr_indent(out, indent);
    // fprintf(out, "tau-t%s" ? " = " : ":");
    // for (i = 0; (i < opts->ngtc); i++)
    // {
    //     fprintf(out, "  %10g", opts->tau_t[i]);
    // }
    // fprintf(out, "\n");
    strategy->pr_rvec_row("tau-t", opts->tau_t, opts->ngtc);
    
    if (!bMDPformat)
    {
        strategy->close_section();
    }

    /* Pretty-print the simulated annealing info */
    // fprintf(out, "annealing%s" ? " = " : ":");
    // for (i = 0; (i < opts->ngtc); i++)
    // {
    //     fprintf(out, "  %10s", enumValueToString(opts->annealing[i]));
    // }
    // fprintf(out, "\n");
    strategy->pr_sim_annealing("annealing", opts->annealing, opts->ngtc);

    // fprintf(out, "annealing-npoints%s" ? " = " : ":");
    // for (i = 0; (i < opts->ngtc); i++)
    // {
    //     fprintf(out, "  %10d", opts->anneal_npoints[i]);
    // }
    // fprintf(out, "\n");
    strategy->pr_ivec_row("annealing-npoints", opts->anneal_npoints, opts->ngtc);

    for (i = 0; (i < opts->ngtc); i++)
    {
        if (opts->anneal_npoints[i] > 0)
        {
            // fprintf(out, "annealing-time [%d]:\t", i);
            // for (j = 0; (j < opts->anneal_npoints[i]); j++)
            // {
            //     fprintf(out, "  %10.1f", opts->anneal_time[i][j]);
            // }
            // fprintf(out, "\n");
            strategy->pr_anneal_points("annealing-time", opts->anneal_time[i], opts->anneal_npoints[i]);
            // fprintf(out, "annealing-temp [%d]:\t", i);
            // for (j = 0; (j < opts->anneal_npoints[i]); j++)
            // {
            //     fprintf(out, "  %10.1f", opts->anneal_temp[i][j]);
            // }
            // fprintf(out, "\n");
            strategy->pr_anneal_points("annealing-temp", opts->anneal_temp[i], opts->anneal_npoints[i]);
        }
    }

    strategy->pr_grp_opt_agg(opts->acceleration, opts->ngacc, opts->nFreeze, opts->ngfrz, opts->egp_flags, opts->ngener);
}

void DumpBuilderInputRec::build(DumpStrategy* strategy) {
    const char* title = "inputrec";
    const char* infbuf = "inf";

    if (strategy->available(ir, title)) {
        if (!bMDPformat) {
            strategy->pr_title(title);
        }
        /* Try to make this list appear in the same order as the
         * options are written in the default mdout.mdp, and with
         * the same user-exposed names to facilitate debugging.
         */
        strategy->pr_named_value("integrator", enumValueToString(ir->eI));
        strategy->pr_named_value("tinit", (real) (ir->init_t));
        strategy->pr_named_value("dt", ir->delta_t);
        strategy->pr_named_value("nsteps", ir->nsteps);
        strategy->pr_named_value("init-step", ir->init_step);
        strategy->pr_named_value("simulation-part", ir->simulation_part);
        DumpBuilderMts(ir->useMts).build(strategy);
        strategy->pr_named_value("mass-repartition-factor", ir->massRepartitionFactor);
        strategy->pr_named_value("comm-mode", enumValueToString(ir->comm_mode));
        strategy->pr_named_value("nstcomm", ir->nstcomm);

        /* Langevin dynamics */
        strategy->pr_named_value("bd-fric", ir->bd_fric);
        strategy->pr_named_value("ld-seed", ir->ld_seed);

        /* Energy minimization */
        strategy->pr_named_value("emtol", ir->em_tol);
        strategy->pr_named_value("emstep", ir->em_stepsize);
        strategy->pr_named_value("niter", ir->niter);
        strategy->pr_named_value("fcstep", ir->fc_stepsize);
        strategy->pr_named_value("nstcgsteep", ir->nstcgsteep);
        strategy->pr_named_value("nbfgscorr", ir->nbfgscorr);

        /* Test particle insertion */
        strategy->pr_named_value("rtpi", ir->rtpi);

        /* Output control */
        strategy->pr_named_value("nstxout", ir->nstxout);
        strategy->pr_named_value("nstvout", ir->nstvout);
        strategy->pr_named_value("nstfout", ir->nstfout);
        strategy->pr_named_value("nstlog", ir->nstlog);
        strategy->pr_named_value("nstcalcenergy", ir->nstcalcenergy);
        strategy->pr_named_value("nstenergy", ir->nstenergy);
        strategy->pr_named_value("nstxout-compressed", ir->nstxout_compressed);
        strategy->pr_named_value("compressed-x-precision", ir->x_compression_precision);

        /* Neighborsearching parameters */
        strategy->pr_named_value("cutoff-scheme", enumValueToString(ir->cutoff_scheme));
        strategy->pr_named_value("nstlist", ir->nstlist);
        strategy->pr_named_value("pbc", c_pbcTypeNames[ir->pbcType].c_str());
        strategy->pr_named_value("periodic-molecules", gmx::boolToString(ir->bPeriodicMols));
        strategy->pr_named_value("verlet-buffer-tolerance", ir->verletbuf_tol);
        strategy->pr_named_value("verlet-buffer-pressure-tolerance", ir->verletBufferPressureTolerance);
        strategy->pr_named_value("rlist", ir->rlist);

        /* Options for electrostatics and VdW */
        strategy->pr_named_value("coulombtype", enumValueToString(ir->coulombtype));
        strategy->pr_named_value("coulomb-modifier", enumValueToString(ir->coulomb_modifier));
        strategy->pr_named_value("rcoulomb-switch", ir->rcoulomb_switch);
        strategy->pr_named_value("rcoulomb", ir->rcoulomb);
        // strategy->pr_named_value("epsilon-r", ir->epsilon_r != 0 ? ir->epsilon_r : infbuf);
        if (ir->epsilon_r != 0) {
            strategy->pr_named_value("epsilon-r", ir->epsilon_r);
        } else {
            strategy->pr_named_value("epsilon-r", infbuf);
        }
        // strategy->pr_named_value("epsilon-rf", ir->epsilon_rf != 0 ? ir->epsilon_rf : infbuf);
        if (ir->epsilon_rf != 0) {
            strategy->pr_named_value("epsilon-rf", ir->epsilon_rf);
        } else {
            strategy->pr_named_value("epsilon-rf", infbuf);
        }
        strategy->pr_named_value("vdw-type", enumValueToString(ir->vdwtype));
        strategy->pr_named_value("vdw-modifier", enumValueToString(ir->vdw_modifier));
        strategy->pr_named_value("rvdw-switch", ir->rvdw_switch);
        strategy->pr_named_value("rvdw", ir->rvdw);
        strategy->pr_named_value("DispCorr", enumValueToString(ir->eDispCorr));
        strategy->pr_named_value("table-extension", ir->tabext);

        strategy->pr_named_value("fourierspacing", ir->fourier_spacing);
        strategy->pr_named_value("fourier-nx", ir->nkx);
        strategy->pr_named_value("fourier-ny", ir->nky);
        strategy->pr_named_value("fourier-nz", ir->nkz);
        strategy->pr_named_value("pme-order", ir->pme_order);
        strategy->pr_named_value("ewald-rtol", ir->ewald_rtol);
        strategy->pr_named_value("ewald-rtol-lj", ir->ewald_rtol_lj);
        strategy->pr_named_value("lj-pme-comb-rule", enumValueToString(ir->ljpme_combination_rule));
        strategy->pr_named_value("ewald-geometry", enumValueToString(ir->ewald_geometry));
        strategy->pr_named_value("epsilon-surface", ir->epsilon_surface);

        /* Options for weak coupling algorithms */
        strategy->pr_named_value("ensemble-temperature-setting", enumValueToString(ir->ensembleTemperatureSetting));
        if (ir->ensembleTemperatureSetting == EnsembleTemperatureSetting::Constant) {
            strategy->pr_named_value("ensemble-temperature", ir->ensembleTemperature);
        }
        strategy->pr_named_value("tcoupl", enumValueToString(ir->etc));
        strategy->pr_named_value("nsttcouple", ir->nsttcouple);
        strategy->pr_named_value("nh-chain-length", ir->opts.nhchainlength);
        strategy->pr_named_value("print-nose-hoover-chain-variables", gmx::boolToString(ir->bPrintNHChains));

        strategy->pr_named_value("pcoupl", enumValueToString(ir->pressureCouplingOptions.epc));
        if (ir->pressureCouplingOptions.epc != PressureCoupling::No) {
            strategy->pr_named_value("pcoupltype", enumValueToString(ir->pressureCouplingOptions.epct));
            strategy->pr_named_value("nstpcouple", ir->pressureCouplingOptions.nstpcouple);
            strategy->pr_named_value("tau-p", ir->pressureCouplingOptions.tau_p);
            strategy->pr_matrix("compressibility", ir->pressureCouplingOptions.compress);
            strategy->pr_matrix("ref-p", ir->pressureCouplingOptions.ref_p);
        }
        // Refcoord-scaling is also needed for other algorithms that affect the box
        strategy->pr_named_value("refcoord-scaling", enumValueToString(ir->pressureCouplingOptions.refcoord_scaling));

        if (bMDPformat) {
        //     fprintf(fp, "posres-com  = %g %g %g\n", ir->posres_com[XX], ir->posres_com[YY], ir->posres_com[ZZ]);
        //     fprintf(fp, "posres-comB = %g %g %g\n", ir->posres_comB[XX], ir->posres_comB[YY], ir->posres_comB[ZZ]);
        } else {
            strategy->pr_rvec("posres-com", ir->posres_com, DIM);
            strategy->pr_rvec("posres-comB", ir->posres_comB, DIM);
        }

        // /* QMMM */
        strategy->pr_named_value("QMMM", gmx::boolToString(ir->bQMMM));
        strategy->close_section();

        DumpBuilderQmOpts(ir).build(strategy);

        DumpBuilderGrpOpts(&(ir->opts)).build(strategy);
    }
}