#include "gromacs/tools/dump/builders/dump_builders_topology.h"

void DumpBuilderMolblock::build(DumpStrategy* strategy)
{
    strategy->pr_named_value("#molblock", molblock.size());
    for (size_t mb = 0; mb < molblock.size(); mb++)
    {
        strategy->pr_title_n("molblock", mb);
        // fprintf(fp, "%-20s = %d \"%s\"\n", "moltype", molb->type, *(molt[molb->type].name));
        strategy->pr_moltype(molblock[mb].type, *(moltype[molblock[mb].type].name));
        strategy->pr_named_value("#molecules", molblock[mb].nmol);
        strategy->pr_named_value("#posres_xA", molblock[mb].posres_xA.size());
        if (!molblock[mb].posres_xA.empty())
        {
            strategy->pr_rvecs("#posres_xA", as_rvec_array(molblock[mb].posres_xA.data()), molblock[mb].posres_xA.size());
        }
        strategy->pr_named_value("#posres_xB", molblock[mb].posres_xB.size());
        if (!molblock[mb].posres_xB.empty())
        {
            strategy->pr_rvecs("#posres_xB", as_rvec_array(molblock[mb].posres_xB.data()), molblock[mb].posres_xB.size());
        }
        strategy->close_section();
    }
}

void DumpBuilderGroups::build(DumpStrategy* strategy)
{
    strategy->pr_grps(groups.groups, groups.groupNames.data());
    // pr_strings(fp, indent, "grpname", groups.groupNames.data(), groups.groupNames.size(), bShowNumbers);
    strategy->pr_groups(groups);

    // pr_indent(fp, indent);
    // fprintf(fp, "groups          ");
    // for (const auto group : gmx::EnumerationWrapper<SimulationAtomGroupType>{})
    // {
    //     printf(" %5.5s", shortName(group));
    // }
    // printf("\n");

    // pr_indent(fp, indent);
    // fprintf(fp, "allocated       ");
    // int nat_max = 0;
    // for (auto group : keysOf(groups.groups))
    // {
    //     printf(" %5d", groups.numberOfGroupNumbers(group));
    //     nat_max = std::max(nat_max, groups.numberOfGroupNumbers(group));
    // }
    // printf("\n");

    // if (nat_max == 0)
    // {
    //     pr_indent(fp, indent);
    //     fprintf(fp, "groupnr[%5s] =", "*");
    //     for (auto gmx_unused group : keysOf(groups.groups))
    //     {
    //         fprintf(fp, "  %3d ", 0);
    //     }
    //     fprintf(fp, "\n");
    // }
    // else
    // {
    //     for (int i = 0; i < nat_max; i++)
    //     {
    //         pr_indent(fp, indent);
    //         fprintf(fp, "groupnr[%5d] =", i);
    //         for (auto group : keysOf(groups.groups))
    //         {
    //             fprintf(fp,
    //                     "  %3d ",
    //                     !groups.groupNumbers[group].empty() ? groups.groupNumbers[group][i] : 0);
    //         }
    //         fprintf(fp, "\n");
    //     }
    // }
}

void DumpBuilderMTop::build(DumpStrategy* strategy)
{
    if (!(strategy->available(mtop, "topology")))
    {
        return;
    }

    strategy->pr_title("topology");
    // fprintf(fp, "name=\"%s\"\n", *(mtop->name));
    strategy->pr_name(*(mtop->name));
    strategy->pr_named_value("#atoms", mtop->natoms);
    DumpBuilderMolblock(mtop->molblock, mtop->moltype).build(strategy);
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
    DumpBuilderFFParams(mtop->ffparams).build(strategy);
    // pr_ffparams(fp, indent, "ffparams", &(mtop->ffparams), bShowNumbers);
    // for (size_t mt = 0; mt < mtop->moltype.size(); mt++)
    for (size_t mt = 0; mt < mtop->moltype.size(); mt++)
    {
        DumpBuilderMoltype(&(mtop->moltype[mt]), mt, mtop->ffparams).build(strategy);
        // pr_moltype(fp, indent, "moltype", &mtop->moltype[mt], mt, &mtop->ffparams, bShowNumbers, bShowParameters);
    }
    // pr_groups(fp, indent, mtop->groups, bShowNumbers);
    DumpBuilderGroups(mtop->groups, bShowNumbers).build(strategy);
    strategy->close_section();
}

void DumpBuilderTop::build(DumpStrategy* strategy)
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

void DumpBuilderFFParams::build(DumpStrategy* strategy)
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

void DumpBuilderMoltype::build(DumpStrategy* strategy)
{
    strategy->pr_title_i("moltype", index);
    strategy->pr_attribute_quoted("name", moltype->name[index]);
    strategy->pr_atoms(&(moltype->atoms));
    DumpBuilderListOfLists("excls", moltype->excls).build(strategy);
    for (int i = 0; (i < F_NRE); i++)
    {
        strategy->pr_interaction_list(
            interaction_function[i].longname,
            ffparams.functype.data(),
            moltype->ilist[i],
            ffparams.iparams.data()
        );
    }
    // pr_listoflists(fp, indent, "excls", &molt->excls, bShowNumbers);
    // for (int j = 0; (j < F_NRE); j++)
    // {
    //     pr_ilist(fp,
    //              indent,
    //              interaction_function[j].longname,
    //              ffparams->functype.data(),
    //              molt->ilist[j],
    //              bShowNumbers,
    //              bShowParameters,
    //              ffparams->iparams.data());
    // }
    strategy->close_section();
}

void DumpBuilderAtoms::build(DumpStrategy* strategy)
{
}

void DumpBuilderListOfLists::build(DumpStrategy* strategy)
{
    if (!strategy->available(&lists, title.c_str()))
    {
        return;
    }

    strategy->pr_title(title.c_str());
    strategy->pr_attribute("numLists", lists.size());
    strategy->pr_attribute("numElements", lists.numElements());
    // indent = pr_title(fp, indent, title);
    // pr_indent(fp, indent);
    // fprintf(fp, "numLists=%zu\n", lists->size());
    // pr_indent(fp, indent);
    // fprintf(fp, "numElements=%d\n", lists->numElements());

    for (gmx::Index i = 0; i < lists.ssize(); i++)
    {
        // gmx::ArrayRef<const int> list = (*lists)[i];
        strategy->pr_list_i(title.c_str(), i, lists[i]);
        // int                      size = pr_indent(fp, indent);
        // gmx::ArrayRef<const int> list = (*lists)[i];
        // if (list.empty())
        // {
        //     size += fprintf(fp, "%s[%d]={", title, int(i));
        // }
        // else
        // {
        //     size += fprintf(fp, "%s[%d][num=%zu]={", title, bShowNumbers ? int(i) : -1, list.size());
        // }
        // bool isFirst = true;
        // for (const int j : list)
        // {
        //     if (!isFirst)
        //     {
        //         size += fprintf(fp, ", ");
        //     }
        //     if ((size) > (USE_WIDTH))
        //     {
        //         fprintf(fp, "\n");
        //         size = pr_indent(fp, indent + INDENT);
        //     }
        //     size += fprintf(fp, "%d", j);
        //     isFirst = false;
        // }
        // fprintf(fp, "}\n");
    }
    strategy->close_section();
}

void DumpBuilderInteractionList::build(DumpStrategy* strategy)
{
    // strategy->pr_title(title.c_str());
    // strategy->pr_attribute("nr", ilist.size());

    if (ilist.empty())
    {
        return;
    }

    // strategy->pr_iatoms("iatoms");
    // indent = pr_title(fp, indent, title);
    // pr_indent(fp, indent);
    // fprintf(fp, "nr: %d\n", ilist.size());
    // if (!ilist.empty())
    // {
    //     pr_indent(fp, indent);
    //     fprintf(fp, "iatoms:\n");
    //     int j = 0;
    //     for (int i = 0; i < ilist.size();)
    //     {
    //         pr_indent(fp, indent + INDENT);
    //         const int type  = ilist.iatoms[i];
    //         const int ftype = functype[type];
    //         if (bShowNumbers)
    //         {
    //             fprintf(fp, "%d type=%d ", j, type);
    //         }
    //         j++;
    //         printf("(%s)", interaction_function[ftype].name);
    //         for (int k = 0; k < interaction_function[ftype].nratoms; k++)
    //         {
    //             fprintf(fp, " %3d", ilist.iatoms[i + 1 + k]);
    //         }
    //         if (bShowParameters)
    //         {
    //             fprintf(fp, "  ");
    //             pr_iparams(fp, ftype, iparams[type]);
    //         }
    //         fprintf(fp, "\n");
    //         i += 1 + interaction_function[ftype].nratoms;
    //     }
    // }
}
