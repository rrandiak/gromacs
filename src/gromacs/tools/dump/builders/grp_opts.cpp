#include "grp_opts.h"

void GroupOptionsBuilder::build(DumpStrategy* strategy) {
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