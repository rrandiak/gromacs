#include "awh.h"

void AwhBuilder::build(DumpStrategy* strategy)
{
    strategy->pr_named_value("awh-potential", enumValueToString(awhParams->potential()));
    strategy->pr_named_value("awh-seed", awhParams->seed());
    strategy->pr_named_value("awh-nstout", awhParams->nstout());
    strategy->pr_named_value("awh-nstsample", awhParams->nstSampleCoord());
    strategy->pr_named_value("awh-nsamples-update", awhParams->numSamplesUpdateFreeEnergy());
    strategy->pr_named_value("awh-share-bias-multisim", gmx::boolToString(awhParams->shareBiasMultisim()));
    strategy->pr_named_value("awh-nbias", awhParams->numBias());

    int index = 1;
    for (const auto& awhBiasParams : awhParams->awhBiasParams())
    {
        AwhBiasBuilder(index, &awhBiasParams).build(strategy);
        index++;
    }
}

void AwhBiasBuilder::build(DumpStrategy* strategy)
{
    strategy->pr_named_value(
        gmx::formatString("awh%d-error-init", index),
        awhBiasParams->initialErrorEstimate()
    );
    strategy->pr_named_value(
        gmx::formatString("awh%d-growth", index),
        enumValueToString(awhBiasParams->growthType())
    );
    strategy->pr_named_value(
        gmx::formatString("awh%d-growth-factor", index),
        awhBiasParams->growthFactor()
    );
    strategy->pr_named_value(
        gmx::formatString("awh%d-target", index),
        enumValueToString(awhBiasParams->targetDistribution())
    );
    strategy->pr_named_value(
        gmx::formatString("awh%d-target-beta-scaling", index),
        awhBiasParams->targetBetaScaling()
    );
    strategy->pr_named_value(
        gmx::formatString("awh%d-target-cutoff", index),
        awhBiasParams->targetCutoff()
    );
    strategy->pr_named_value(
        gmx::formatString("awh%d-target-metric-scaling", index),
        gmx::boolToString(awhBiasParams->scaleTargetByMetric())
    );
    strategy->pr_named_value(
        gmx::formatString("awh%d-target-metric-scaling-limit", index),
        awhBiasParams->targetMetricScalingLimit()
    );
    strategy->pr_named_value(
        gmx::formatString("awh%d-user-data", index),
        gmx::boolToString(awhBiasParams->userPMFEstimate())
    );
    strategy->pr_named_value(
        gmx::formatString("awh%d-share-group", index),
        awhBiasParams->shareGroup()
    );
    strategy->pr_named_value(
        gmx::formatString("awh%d-equilibrate-histogram", index),
        gmx::boolToString(awhBiasParams->equilibrateHistogram())
    );
    strategy->pr_named_value(
        gmx::formatString("awh%d-ndim", index),
        awhBiasParams->ndim()
    );

    int dim = 1;
    for (const auto& dimParams : awhBiasParams->dimParams())
    {
        AwhBiasDimBuilder(index, dim, &dimParams).build(strategy);
        dim++;
    }
}

void AwhBiasDimBuilder::build(DumpStrategy* strategy)
{
    strategy->pr_title_awh(gmx::formatString("awh%d-dim%d", index, dimension));
    strategy->pr_named_value("coord-provider", enumValueToString(awhDimParams->coordinateProvider()));
    strategy->pr_named_value("coord-index", awhDimParams->coordinateIndex() + 1);
    strategy->pr_named_value("start", awhDimParams->origin());
    strategy->pr_named_value("end", awhDimParams->end());
    strategy->pr_named_value("period", awhDimParams->period());
    strategy->pr_named_value("force-constant", awhDimParams->forceConstant());
    strategy->pr_named_value("diffusion", awhDimParams->diffusion());
    strategy->pr_named_value("cover-diameter", awhDimParams->coverDiameter());
    strategy->close_section();
}