#include "topology.h"
#include "gromacs/topology/idef.h"

#include "gromacs/utility/strconvert.h"

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

    if (!(mtop->moltype.empty()))
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

void AtomsBuilder::build(DumpStrategy* strategy)
{
    strategy->pr_title("atoms");

    if (strategy->available(atoms->atom, "atom"))
    {
        strategy->pr_title_n("atom", atoms->nr);
        for (int i = 0; (i < atoms->nr); i++)
        {
            strategy->pr_atom(&(atoms->atom[i]), i);
        }
        strategy->close_section();

        strategy->pr_title_atom_names(atoms->nr);
        const char* atomAttributeNames[] = {"name"};
        for (int i = 0; (i < atoms->nr); i++)
        {
            strategy->pr_vec_attributes("atom", i,  atomAttributeNames, atoms->atomname[i], 1);
        }
        strategy->close_section();

        strategy->pr_title_n("type", atoms->nr);
        const char* typeAttributeNames[] = {"name", "nameB"};
        for (int i = 0; (i < atoms->nr); i++)
        {
            char* values[] = {*(atoms->atomtype[i]), *(atoms->atomtypeB[i])};
            strategy->pr_vec_attributes("type", i,  typeAttributeNames, values, 2);
        }
        strategy->close_section();

        if (strategy->available(atoms->resinfo, "residue"))
        {
            strategy->pr_title_n("residue", atoms->nres);
            for (int i = 0; i < atoms->nres; i++)
            {
                strategy->pr_residue(&(atoms->resinfo[i]), i);
            }
            strategy->close_section();
        }

    }

    strategy->close_section();
}

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

void ListOfListsBuilder::build(DumpStrategy* strategy)
{
    if (!strategy->available(&lists, title))
    {
        return;
    }

    strategy->pr_title(title);
    strategy->pr_attribute("numLists", lists.size());
    strategy->pr_attribute("numElements", lists.numElements());

    strategy->pr_title_list(title);

    for (gmx::Index i = 0; i < lists.ssize(); i++)
    {
        strategy->pr_list_i(title, i, lists[i]);
    }

    strategy->close_list();
    strategy->close_section();
}

void MolblockBuilder::build(DumpStrategy* strategy)
{
    strategy->pr_count("molblock", molblock.size());
    strategy->pr_title_list("molblock");
    for (size_t mb = 0; mb < molblock.size(); mb++)
    {
        strategy->pr_title_i("molblock", mb);
        strategy->pr_moltype(molblock[mb].type, *(moltype[molblock[mb].type].name));
        strategy->pr_count("molecules", molblock[mb].nmol);
        strategy->pr_count("posres_xA", molblock[mb].posres_xA.size());
        if (!molblock[mb].posres_xA.empty())
        {
            strategy->pr_rvecs("posres_xA", as_rvec_array(molblock[mb].posres_xA.data()), molblock[mb].posres_xA.size());
        }
        strategy->pr_count("posres_xB", molblock[mb].posres_xB.size());
        if (!molblock[mb].posres_xB.empty())
        {
            strategy->pr_rvecs("posres_xB", as_rvec_array(molblock[mb].posres_xB.data()), molblock[mb].posres_xB.size());
        }
        strategy->close_section();
    }
    strategy->close_list();
}

void MoltypeBuilder::build(DumpStrategy* strategy)
{
    strategy->pr_title_i("moltype", index);
    strategy->pr_attribute_quoted("name", *(moltype->name));
    AtomsBuilder(&(moltype->atoms)).build(strategy);
    ListOfListsBuilder("excls", moltype->excls).build(strategy);
    for (int i = 0; (i < F_NRE); i++)
    {
        strategy->pr_interaction_list(
            interaction_function[i].longname,
            ffparams.functype.data(),
            moltype->ilist[i],
            ffparams.iparams.data()
        );
    }
    strategy->close_section();
}

void SimulationGroupsBuilder::build(DumpStrategy* strategy)
{
    strategy->pr_grps(groups.groups, groups.groupNames.data());

    strategy->pr_title_n("grpname", groups.groupNames.size());
    const char* names[] = {"name"};
    for (size_t i = 0; i < groups.groupNames.size(); i++)
    {
        strategy->pr_vec_attributes("grpname", i, names, groups.groupNames[i], 1);
    }
    strategy->close_section();

    strategy->pr_groups(groups);
}
