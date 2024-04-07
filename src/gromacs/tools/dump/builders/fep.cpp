#include "gromacs/tools/dump/builders/fep.h"

void FepBuilder::build(DumpStrategy* strategy)
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
        // TODO: finish
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
