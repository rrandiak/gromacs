#include "topology.h"
#include "gromacs/topology/idef.h"


void TopologyBuilder::build(DumpStrategy* strategy)
{
    if (!strategy->bSysTop)
    {
        MTopBuilder(mtop).build(strategy);
    }
    else
    {
        t_topology top = gmx_mtop_t_to_t_topology(mtop, false);
        TopBuilder(&top).build(strategy);
    }
}

void MTopBuilder::build(DumpStrategy* strategy)
{
    if (!(strategy->available(mtop, "topology")))
    {
        return;
    }

    strategy->pr_title("topology");
    strategy->pr_attribute_quoted("name", *(mtop->name));
    strategy->pr_count("atoms", mtop->natoms);

    MolblockBuilder(mtop->molblock, mtop->moltype).build(strategy);

    strategy->pr_named_value("bIntermolecularInteractions", gmx::boolToString(mtop->bIntermolecularInteractions));

    FFParamsBuilder(mtop->ffparams).build(strategy);

    if (mtop->moltype.size() > 0)
    {
        strategy->pr_title_list("moltype");
        for (size_t mt = 0; mt < mtop->moltype.size(); mt++)
        {
            MoltypeBuilder(&(mtop->moltype[mt]), mt, mtop->ffparams).build(strategy);
        }
        strategy->close_list();
    }

    SimulationGroupsBuilder(mtop->groups).build(strategy);
    strategy->close_section();
}

void TopBuilder::build(DumpStrategy* strategy)
{
    if (!(strategy->available(top, "topology")))
    {
        return;
    }

    strategy->pr_title("topology");
    strategy->pr_attribute_quoted("name", *(top->name));
    AtomsBuilder(&(top->atoms)).build(strategy);
    strategy->pr_block("mols", &top->mols);
    strategy->pr_named_value("bIntermolecularInteractions", gmx::boolToString(top->bIntermolecularInteractions));
    if (strategy->available(&top->idef, "idef"))
    {
        strategy->pr_attribute("atnr=%d", top->idef.atnr);
        strategy->pr_attribute("ntypes=%d", top->idef.ntypes);
        strategy->pr_functypes(top->idef.functype, top->idef.ntypes, top->idef.iparams);
        strategy->pr_named_value("fudgeQQ", top->idef.fudgeQQ);

        for (int j = 0; (j < F_NRE); j++)
        {
            InteractionList iList = InteractionList();
            iList.push_back(top->idef.il[j].nalloc, top->idef.il[j].nr, top->idef.il[j].iatoms);
            strategy->pr_interaction_list(
                interaction_function[j].longname,
                top->idef.functype,
                iList,
                top->idef.iparams
            );
        }
    }
}
