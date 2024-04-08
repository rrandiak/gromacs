#include "topology.h"



void TopologyBuilder::build(DumpStrategy* strategy)
{
    // TODO: bSysTop to settings
    bool bSysTop = false;
    if (!bSysTop)
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
    // fprintf(fp, "name=\"%s\"\n", *(mtop->name));
    // strategy->pr_name(*(mtop->name));
    strategy->pr_attribute_quoted("name", *(mtop->name));
    strategy->pr_named_value("#atoms", mtop->natoms);
    MolblockBuilder(mtop->molblock, mtop->moltype).build(strategy);
    strategy->pr_named_value("bIntermolecularInteractions", gmx::boolToString(mtop->bIntermolecularInteractions));
    // if (mtop->bIntermolecularInteractions)
    // {
    //     for (int j = 0; j < F_NRE; j++)
    //     {
    //         pr_ilist(fp,
    //                     indent,
    //                     interaction_function[j].longname,
    //                     mtop->ffparams.functype.data(),
    //                     (*mtop->intermolecular_ilist)[j],
    //                     bShowNumbers,
    //                     bShowParameters,
    //                     mtop->ffparams.iparams.data());
    //     }
    // }
    FFParamsBuilder(mtop->ffparams).build(strategy);
    // pr_ffparams(fp, indent, "ffparams", &(mtop->ffparams), bShowNumbers);
    // for (size_t mt = 0; mt < mtop->moltype.size(); mt++)
    for (size_t mt = 0; mt < mtop->moltype.size(); mt++)
    {
        MoltypeBuilder(&(mtop->moltype[mt]), mt, mtop->ffparams).build(strategy);
        // pr_moltype(fp, indent, "moltype", &mtop->moltype[mt], mt, &mtop->ffparams, bShowNumbers, bShowParameters);
    }
    // pr_groups(fp, indent, mtop->groups, bShowNumbers);
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
    // fprintf(fp, "name=\"%s\"\n", *(top->name));
    // pr_atoms(fp, indent, "atoms", &(top->atoms), bShowNumbers);
    // pr_block(fp, indent, "mols", &top->mols, bShowNumbers);
    strategy->pr_named_value("bIntermolecularInteractions", gmx::boolToString(top->bIntermolecularInteractions));
    // pr_idef(fp, indent, "idef", &top->idef, bShowNumbers, bShowParameters);
}
