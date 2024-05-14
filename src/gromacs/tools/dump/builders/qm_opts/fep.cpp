#include "fep.h"
#include "gromacs/mdtypes/md_enums.h"
#include "gromacs/topology/symtab.h"

void FepBuilder::build(DumpStrategy* strategy)
{
    int j;

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
        // TODO: bMDPformat
        strategy->pr_title("separate-dvdl");
        for (auto i : gmx::EnumerationArray<FreeEnergyPerturbationCouplingType, bool>::keys())
        {
            strategy->pr_separate_dvdl(enumValueToString(i), fep->separate_dvdl[i]);
        }
        strategy->close_section();

        // TODO: bMDPformat
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
};
