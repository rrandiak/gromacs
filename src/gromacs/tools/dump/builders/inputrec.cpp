#include "inputrec.h"

void InputRecBuilder::build(DumpStrategy* strategy) {
    const char* title = "inputrec";
    const char* infbuf = "inf";

    if (strategy->available(ir, title)) {
        if (!strategy->bMDPformat) {
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
        MtsBuilder(ir->useMts).build(strategy);
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
        if (ir->epsilon_r != 0) {
            strategy->pr_named_value("epsilon-r", ir->epsilon_r);
        } else {
            strategy->pr_named_value("epsilon-r", infbuf);
        }
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

        // TODO: pr_dim_rvec
        if (strategy->bMDPformat) {
        //     fprintf(fp, "posres-com  = %g %g %g\n", ir->posres_com[XX], ir->posres_com[YY], ir->posres_com[ZZ]);
        //     fprintf(fp, "posres-comB = %g %g %g\n", ir->posres_comB[XX], ir->posres_comB[YY], ir->posres_comB[ZZ]);
        } else {
            strategy->pr_rvec("posres-com", ir->posres_com, DIM);
            strategy->pr_rvec("posres-comB", ir->posres_comB, DIM);
        }

        // /* QMMM */
        strategy->pr_named_value("QMMM", gmx::boolToString(ir->bQMMM));

        if (!strategy->bMDPformat) {
            strategy->close_section();
        }
    }
}

void MtsBuilder::build(DumpStrategy* strategy)
{
    // TODO: finish
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