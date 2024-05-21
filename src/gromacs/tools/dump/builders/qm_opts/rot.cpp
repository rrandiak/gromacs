#include "rot.h"

void RotBuilder::build(DumpStrategy* strategy)
{
    int g;

    strategy->pr_named_value("rot-nstrout", rot->nstrout);
    strategy->pr_named_value("rot-nstsout", rot->nstsout);
    strategy->pr_named_value("rot-ngroups", rot->grp.size());
    for (g = 0; g < gmx::ssize(rot->grp); g++)
    {
        RotGroupBuilder(g, &(rot->grp[g])).build(strategy);
    }
}

void RotGroupBuilder::build(DumpStrategy* strategy)
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