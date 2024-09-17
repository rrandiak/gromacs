#include "grp_opts.h"

void GroupOptionsBuilder::build(DumpStrategy* strategy) {
    if (!strategy->bMDPformat)
    {
        strategy->pr_title("grpopts");
    }

    strategy->pr_rvec_row("nrdf", opts->nrdf, opts->ngtc);

    strategy->pr_rvec_row("ref-t", opts->ref_t, opts->ngtc);

    strategy->pr_rvec_row("tau-t", opts->tau_t, opts->ngtc);
    
    if (!strategy->bMDPformat)
    {
        strategy->close_section();
    }

    /* Pretty-print the simulated annealing info */
    strategy->pr_sa_vec_row("annealing", opts->annealing, opts->ngtc);

    strategy->pr_ivec_row("annealing-npoints", opts->anneal_npoints, opts->ngtc);

    for (int i = 0; (i < opts->ngtc); i++)
    {
        if (opts->anneal_npoints[i] > 0)
        {
            // strategy->pr_ap_vec_row("annealing-time", opts->anneal_time[i], opts->anneal_npoints[i]);
            strategy->pr_ap_vec_row("annealing-time", opts->anneal_time[i], opts->anneal_npoints[i], i);
            // strategy->pr_ap_vec_row("annealing-temp", opts->anneal_temp[i], opts->anneal_npoints[i]);
            strategy->pr_ap_vec_row("annealing-temp", opts->anneal_temp[i], opts->anneal_npoints[i], i);
        }
    }

    strategy->pr_grp_opt_agg(
        opts->acceleration, opts->ngacc,
        opts->nFreeze, opts->ngfrz,
        opts->egp_flags, opts->ngener
    );
}