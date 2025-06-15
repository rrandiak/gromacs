#include "tpx_header.h"

void TpxHeaderRepr::build(ReprFormatter* strategy)
{
    if (!strategy->available(sh, "header"))
    {
        return;
    }

    strategy->pr_title("header");

    strategy->pr_named_value_short_format("bIr", sh->bIr ? "present" : "not present");
    strategy->pr_named_value_short_format("bBox", sh->bBox ? "present" : "not present");
    strategy->pr_named_value_short_format("bTop", sh->bTop ? "present" : "not present");
    strategy->pr_named_value_short_format("bX", sh->bX ? "present" : "not present");
    strategy->pr_named_value_short_format("bV", sh->bV ? "present" : "not present");
    strategy->pr_named_value_short_format("bF", sh->bF ? "present" : "not present");
    strategy->pr_named_value_short_format("natoms", sh->natoms);
    strategy->pr_named_value_scientific("lambda", sh->lambda);
    strategy->pr_named_value_short_format("buffer size", sh->sizeOfTprBody);

    strategy->close_section();
}
