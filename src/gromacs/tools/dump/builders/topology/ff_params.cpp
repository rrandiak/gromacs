#include "ff_params.h"

void FFParamsBuilder::build(DumpStrategy* strategy)
{
    strategy->pr_title("ffparams");
    strategy->pr_attribute("atnr", ffparams.atnr);
    strategy->pr_attribute("ntypes", ffparams.numTypes());
    strategy->pr_functypes(ffparams.functype.data(), ffparams.numTypes(), ffparams.iparams.data());
    strategy->pr_named_value("reppow", ffparams.reppow);
    strategy->pr_named_value("fudgeQQ", ffparams.fudgeQQ);
    strategy->pr_cmap(&(ffparams.cmap_grid));
    strategy->close_section();
}
