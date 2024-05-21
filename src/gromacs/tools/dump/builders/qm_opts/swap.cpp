#include "swap.h"

void SwapBuilder::build(DumpStrategy* strategy)
{
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
        strategy->pr_named_value(
            gmx::formatString("massw_split%d", j),
            gmx::boolToString(swap->massw_split[j])
        );
        strategy->pr_ivec_block(
            gmx::formatString("split atoms group %d", j),
            swap->grp[j].ind,
            swap->grp[j].nat,
            TRUE
        );
    }

    /* The solvent group */
    strategy->pr_ivec_block(
        gmx::formatString("solvent group %s", swap->grp[static_cast<int>(SwapGroupSplittingType::Solvent)].molname),
        swap->grp[static_cast<int>(SwapGroupSplittingType::Solvent)].ind,
        swap->grp[static_cast<int>(SwapGroupSplittingType::Solvent)].nat,
        TRUE
    );

    /* Now print the indices for all the ion groups: */
    for (int ig = static_cast<int>(SwapGroupSplittingType::Count); ig < swap->ngrp; ig++)
    {
        strategy->pr_ivec_block(
            gmx::formatString("ion group %s", swap->grp[ig].molname),
            swap->grp[ig].ind,
            swap->grp[ig].nat,
            TRUE
        );
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
            strategy->pr_named_value(
                gmx::formatString("%s-in-%c", swap->grp[ig].molname, 'A' + ic),
                swap->grp[ig].nmolReq[ic]
            );
        }
    }

    strategy->pr_named_value("threshold", swap->threshold);
    strategy->pr_named_value("bulk-offsetA", swap->bulkOffset[eCompA]);
    strategy->pr_named_value("bulk-offsetB", swap->bulkOffset[eCompB]);
}
