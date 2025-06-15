#include "inputrec.h"

#include "gromacs/math/vectypes.h"

void InputRecRepr::build(ReprFormatter* strategy)
{
    const char* title  = "inputrec";
    const char* infbuf = "inf";

    if (strategy->available(ir, title))
    {
        if (!strategy->bMDPformat)
        {
            strategy->pr_title(title);
        }
        /* Try to make this list appear in the same order as the
         * options are written in the default mdout.mdp, and with
         * the same user-exposed names to facilitate debugging.
         */
        strategy->pr_key_value("integrator", enumValueToString(ir->eI));
        strategy->pr_key_value("tinit", (real)(ir->init_t));
        strategy->pr_key_value("dt", ir->delta_t);
        strategy->pr_key_value("nsteps", ir->nsteps);
        strategy->pr_key_value("init-step", ir->init_step);
        strategy->pr_key_value("simulation-part", ir->simulation_part);
        MtsRepr(ir->useMts, ir->mtsLevels).build(strategy);
        strategy->pr_key_value("mass-repartition-factor", ir->massRepartitionFactor);
        strategy->pr_key_value("comm-mode", enumValueToString(ir->comm_mode));
        strategy->pr_key_value("nstcomm", ir->nstcomm);

        /* Langevin dynamics */
        strategy->pr_key_value("bd-fric", ir->bd_fric);
        strategy->pr_key_value("ld-seed", ir->ld_seed);

        /* Energy minimization */
        strategy->pr_key_value("emtol", ir->em_tol);
        strategy->pr_key_value("emstep", ir->em_stepsize);
        strategy->pr_key_value("niter", ir->niter);
        strategy->pr_key_value("fcstep", ir->fc_stepsize);
        strategy->pr_key_value("nstcgsteep", ir->nstcgsteep);
        strategy->pr_key_value("nbfgscorr", ir->nbfgscorr);

        /* Test particle insertion */
        strategy->pr_key_value("rtpi", ir->rtpi);

        /* Output control */
        strategy->pr_key_value("nstxout", ir->nstxout);
        strategy->pr_key_value("nstvout", ir->nstvout);
        strategy->pr_key_value("nstfout", ir->nstfout);
        strategy->pr_key_value("nstlog", ir->nstlog);
        strategy->pr_key_value("nstcalcenergy", ir->nstcalcenergy);
        strategy->pr_key_value("nstenergy", ir->nstenergy);
        strategy->pr_key_value("nstxout-compressed", ir->nstxout_compressed);
        strategy->pr_key_value("compressed-x-precision", ir->x_compression_precision);

        /* Neighborsearching parameters */
        strategy->pr_key_value("cutoff-scheme", enumValueToString(ir->cutoff_scheme));
        strategy->pr_key_value("nstlist", ir->nstlist);
        strategy->pr_key_value("pbc", c_pbcTypeNames[ir->pbcType].c_str());
        strategy->pr_key_value("periodic-molecules", gmx::boolToString(ir->bPeriodicMols));
        strategy->pr_key_value("verlet-buffer-tolerance", ir->verletbuf_tol);
        strategy->pr_key_value("verlet-buffer-pressure-tolerance", ir->verletBufferPressureTolerance);
        strategy->pr_key_value("rlist", ir->rlist);

        /* Options for electrostatics and VdW */
        strategy->pr_key_value("coulombtype", enumValueToString(ir->coulombtype));
        strategy->pr_key_value("coulomb-modifier", enumValueToString(ir->coulomb_modifier));
        strategy->pr_key_value("rcoulomb-switch", ir->rcoulomb_switch);
        strategy->pr_key_value("rcoulomb", ir->rcoulomb);
        if (ir->epsilon_r != 0)
        {
            strategy->pr_key_value("epsilon-r", ir->epsilon_r);
        }
        else
        {
            strategy->pr_key_value("epsilon-r", infbuf);
        }
        if (ir->epsilon_rf != 0)
        {
            strategy->pr_key_value("epsilon-rf", ir->epsilon_rf);
        }
        else
        {
            strategy->pr_key_value("epsilon-rf", infbuf);
        }
        strategy->pr_key_value("vdw-type", enumValueToString(ir->vdwtype));
        strategy->pr_key_value("vdw-modifier", enumValueToString(ir->vdw_modifier));
        strategy->pr_key_value("rvdw-switch", ir->rvdw_switch);
        strategy->pr_key_value("rvdw", ir->rvdw);
        strategy->pr_key_value("DispCorr", enumValueToString(ir->eDispCorr));
        strategy->pr_key_value("table-extension", ir->tabext);

        strategy->pr_key_value("fourierspacing", ir->fourier_spacing);
        strategy->pr_key_value("fourier-nx", ir->nkx);
        strategy->pr_key_value("fourier-ny", ir->nky);
        strategy->pr_key_value("fourier-nz", ir->nkz);
        strategy->pr_key_value("pme-order", ir->pme_order);
        strategy->pr_key_value("ewald-rtol", ir->ewald_rtol);
        strategy->pr_key_value("ewald-rtol-lj", ir->ewald_rtol_lj);
        strategy->pr_key_value("lj-pme-comb-rule", enumValueToString(ir->ljpme_combination_rule));
        strategy->pr_key_value("ewald-geometry", enumValueToString(ir->ewald_geometry));
        strategy->pr_key_value("epsilon-surface", ir->epsilon_surface);

        /* Options for weak coupling algorithms */
        strategy->pr_key_value("ensemble-temperature-setting",
                               enumValueToString(ir->ensembleTemperatureSetting));
        if (ir->ensembleTemperatureSetting == EnsembleTemperatureSetting::Constant)
        {
            strategy->pr_key_value("ensemble-temperature", ir->ensembleTemperature);
        }
        strategy->pr_key_value("tcoupl", enumValueToString(ir->etc));
        strategy->pr_key_value("nsttcouple", ir->nsttcouple);
        strategy->pr_key_value("nh-chain-length", ir->opts.nhchainlength);
        strategy->pr_key_value("print-nose-hoover-chain-variables", gmx::boolToString(ir->bPrintNHChains));

        strategy->pr_key_value("pcoupl", enumValueToString(ir->pressureCouplingOptions.epc));
        if (ir->pressureCouplingOptions.epc != PressureCoupling::No)
        {
            strategy->pr_key_value("pcoupltype", enumValueToString(ir->pressureCouplingOptions.epct));
            strategy->pr_key_value("nstpcouple", ir->pressureCouplingOptions.nstpcouple);
            strategy->pr_key_value("tau-p", ir->pressureCouplingOptions.tau_p);
            strategy->pr_matrix("compressibility", ir->pressureCouplingOptions.compress);
            strategy->pr_matrix("ref-p", ir->pressureCouplingOptions.ref_p);
        }
        // Refcoord-scaling is also needed for other algorithms that affect the box
        strategy->pr_key_value("refcoord-scaling",
                               enumValueToString(ir->pressureCouplingOptions.refcoord_scaling));

        strategy->pr_rvecs("posres-com", gmx::as_rvec_array(ir->posresCom.data()), ir->posresCom.size());
        strategy->pr_rvecs(
                "posres-comB", gmx::as_rvec_array(ir->posresComB.data()), ir->posresComB.size());

        // /* QMMM */
        strategy->pr_key_value("QMMM", gmx::boolToString(ir->bQMMM));

        if (!strategy->bMDPformat)
        {
            strategy->close_section();
        }
    }
}

void MtsRepr::build(ReprFormatter* strategy)
{
    strategy->pr_key_value("mts", gmx::boolToString(useMts));

    if (useMts)
    {
        for (int mtsIndex = 1; mtsIndex < static_cast<int>(mtsLevels.size()); mtsIndex++)
        {
            const auto&       mtsLevel = mtsLevels[mtsIndex];
            const std::string forceKey = gmx::formatString("mts-level%d-forces", mtsIndex + 1);
            std::string       forceGroups;

            for (int i = 0; i < static_cast<int>(gmx::MtsForceGroups::Count); i++)
            {
                if (mtsLevel.forceGroups[i])
                {
                    if (!forceGroups.empty())
                    {
                        forceGroups += " ";
                    }
                    forceGroups += gmx::mtsForceGroupNames[i];
                }
            }

            strategy->pr_key_value(forceKey.c_str(), forceGroups.c_str());
            const std::string factorKey = gmx::formatString("mts-level%d-factor", mtsIndex + 1);
            strategy->pr_key_value(factorKey.c_str(), mtsLevel.stepFactor);
        }
    }
}