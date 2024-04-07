#include "gromacs/tools/dump/builders/expanded.h"

void ExpandedBuilder::build(DumpStrategy* strategy)
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
