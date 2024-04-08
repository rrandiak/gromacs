#include "ff_params.h"

void FFParamsBuilder::build(DumpStrategy* strategy)
{
    strategy->pr_title("ffparams");
    strategy->pr_attribute("atnr", ffparams.atnr);
    strategy->pr_attribute("ntypes", ffparams.numTypes());
    strategy->pr_functypes(ffparams.functype, ffparams.numTypes(), ffparams.iparams);
    strategy->pr_named_value("reppow", ffparams.reppow);
    strategy->pr_named_value("fudgeQQ", ffparams.fudgeQQ);
    strategy->pr_cmap(&(ffparams.cmap_grid));
    strategy->close_section();

    // indent = pr_title(fp, indent, title);
    // pr_indent(fp, indent);
    // fprintf(fp, "atnr=%d\n", ffparams->atnr);
    // pr_indent(fp, indent);
    // fprintf(fp, "ntypes=%d\n", ffparams->numTypes());
    // for (int i = 0; i < ffparams->numTypes(); i++)
    // {
    //     pr_indent(fp, indent + INDENT);
    //     fprintf(fp,
    //             "functype[%d]=%s, ",
    //             bShowNumbers ? i : -1,
    //             interaction_function[ffparams->functype[i]].name);
    //     pr_iparams(fp, ffparams->functype[i], ffparams->iparams[i]);
    // }
    // pr_double(fp, indent, "reppow", ffparams->reppow);
    // pr_real(fp, indent, "fudgeQQ", ffparams->fudgeQQ);
    // pr_cmap(fp, indent, "cmap", &ffparams->cmap_grid, bShowNumbers);
}
