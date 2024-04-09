#include "pull.h"

void PullBuilder::build(DumpStrategy* strategy)
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
        PullGroupBuilder(g, &(pull->group[g])).build(strategy);
    }
    strategy->pr_named_value("pull-ncoords", pull->ncoord);
    for (g = 0; g < pull->ncoord; g++)
    {
        PullCoordBuilder(g, &(pull->coord[g])).build(strategy);
    }
}

void PullGroupBuilder::build(DumpStrategy* strategy)
{
    strategy->pr_title_i("pull-group", index);
    strategy->pr_ivec_block("atom", pgrp->ind.data(), pgrp->ind.size());
    strategy->pr_rvec("weight", pgrp->weight.data(), pgrp->weight.size());
    strategy->pr_named_value("pbcatom", pgrp->pbcatom);
    strategy->close_section();
}

void PullCoordBuilder::build(DumpStrategy* strategy)
{
    strategy->pr_title_i("pull-coord", index);
    strategy->pr_named_value("type", enumValueToString(pcrd->eType));
    if (pcrd->eType == PullingAlgorithm::External)
    {
        strategy->pr_named_value("potential-provider", pcrd->externalPotentialProvider.c_str());
    }
    strategy->pr_named_value("geometry", enumValueToString(pcrd->eGeom));
    for (int g = 0; g < pcrd->ngroup; g++)
    {
        std::string buffer = gmx::formatString("group[%d]", g);
        strategy->pr_named_value(buffer.c_str(), pcrd->group[g]);
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