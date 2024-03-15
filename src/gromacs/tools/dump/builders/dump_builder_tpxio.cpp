#include "gromacs/tools/dump/builders/dump_builder_tpxio.h"

void DumpBuilderTpxHeader::build(DumpStrategy* strategy) 
{
    if (!strategy->available(sh, "header"))
    {
        return;
    }

    strategy->pr_title("header");
    strategy->pr_is_present("bIr", sh->bIr);
    strategy->pr_is_present("bBox", sh->bBox);
    strategy->pr_is_present("bTop", sh->bTop);
    strategy->pr_is_present("bX", sh->bX);
    strategy->pr_is_present("bV", sh->bV);
    strategy->pr_is_present("bF", sh->bF);
    strategy->pr_named_value("natoms", sh->natoms);
    strategy->pr_named_value("lambda", sh->lambda);
    strategy->pr_named_value("buffer size", sh->sizeOfTprBody);
    strategy->close_section();
}