#include "dump_builder_mtop.h"

void DumpBuilderMTop::build(DumpStrategy* strategy)
{
    if (!(strategy->available(mtop, "topology")))
    {
        return;
    }

    Value value;
    strategy->pr_title("topology");
    strategy->pr_attribute_quoted("name", *(mtop->name));
    strategy->pr_named_value("#atoms", mtop->natoms);
    strategy->pr_tpx_header(nullptr);
    strategy->pr_atoms(nullptr);
    DumpBuilderMolblock(mtop->molblock, mtop->moltype).build(strategy);
    strategy->pr_named_value("bIntermolecularInteractions", gmx::boolToString(mtop->bIntermolecularInteractions));
    DumpBuilderFFParams(mtop->ffparams).build(strategy);
    for (size_t mt = 0; mt < mtop->moltype.size(); mt++)
    {
        DumpBuilderMoltype(&(mtop->moltype[mt]), mt, mtop->ffparams).build(strategy);
    }
    DumpBuilderGroups(mtop->groups, bShowNumbers).build(strategy);
    strategy->close_section();
}