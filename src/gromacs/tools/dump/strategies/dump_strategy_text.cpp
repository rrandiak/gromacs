#include "gromacs/tools/dump/strategies/dump_strategy_text.h"

int DumpComponent::indentValue = 3;

DumpStrategyText::DumpStrategyText(FILE* fp)
{
    TextDumpComponent* root = new TextRootComponent(fp);
    componentsStack.push(root);
}

bool DumpStrategyText::available(const void* p, const std::string title)
{
    if (!p) {
        componentsStack.top()->addFormattedTextLeaf("%s: %s", title.c_str(), "not available");
    }
    return (p != nullptr);
}

void DumpStrategyText::pr_filename(const std::string filename)
{
    componentsStack.top()->printFilename(filename);
}

void DumpStrategyText::pr_title(const std::string title)
{
    TextDumpComponent* comp = componentsStack.top()->addTextObject(title);
    componentsStack.push(comp);
}

void DumpStrategyText::pr_title_i(const std::string title, int i)
{
    TextDumpComponent* comp = componentsStack.top()->addTextObject(title, i);
    componentsStack.push(comp);
}

void DumpStrategyText::pr_title_n(const std::string title, int n)
{
    int bufferSize = snprintf(nullptr, 0, "%s (%d)", title.c_str(), n) + 1;
    std::string formattedString(bufferSize, '\0');
    snprintf(&formattedString[0], bufferSize, "%s (%d)", title.c_str(), n);

    TextDumpComponent* comp = componentsStack.top()->addTextObject(formattedString.c_str());
    componentsStack.push(comp);
}

void DumpStrategyText::pr_title_nxn(const std::string title, int n1, int n2)
{
    int bufferSize = snprintf(nullptr, 0, "%s (%dx%d)", title.c_str(), n1, n2) + 1;
    std::string formattedString(bufferSize, '\0');
    snprintf(&formattedString[0], bufferSize, "%s (%dx%d)", title.c_str(), n1, n2);

    TextDumpComponent* comp = componentsStack.top()->addTextObject(formattedString.c_str());
    componentsStack.push(comp);
}

void DumpStrategyText::close_section()
{
    componentsStack.pop();
}

void DumpStrategyText::pr_named_value(const std::string name, const Value& value)
{
    componentsStack.top()->addTextLeaf(name, value);
}

void DumpStrategyText::pr_named_value_short_format(const std::string name, const Value& value)
{
    componentsStack.top()->addAlignedTextLeaf(name, value, 6);
}

void DumpStrategyText::pr_named_value_scientific(const std::string name, const real& value)
{
    componentsStack.top()->addFormattedTextLeaf("%-*s = %e", 6, name.c_str(), value);
}

void DumpStrategyText::pr_attribute(const std::string name, const Value& value)
{
    componentsStack.top()->addAttribute(name, value);
}

void DumpStrategyText::pr_attribute_quoted(const std::string name, const std::string& value)
{
    componentsStack.top()->addFormattedTextLeaf("%s=\"%s\"", name.c_str(), value.c_str());
}
    
void DumpStrategyText::pr_vec_attributes(const std::string title, int i, const char** names, char** values, int n)
{
    bool bShowNumbers = true;
    componentsStack.top()->addFormattedTextLeaf(
        "%s[%d]={", title.c_str(), bShowNumbers ? i : -1
    );
    for (int j = 0; j < n - 1; j++)
    {
        componentsStack.top()->printFormattedText("%s=\"%s\",", names[j], values[j]);
    }
    componentsStack.top()->printFormattedText("%s=\"%s\"}", names[n - 1], values[n - 1]);
}

void DumpStrategyText::pr_ivec(const std::string title, const int vec[], int n)
{
}

void DumpStrategyText::pr_ivec_row(const std::string title, const int vec[], int n)
{
    // fprintf(out, "annealing-npoints%s", bMDPformat ? " = " : ":");
    // for (i = 0; (i < opts->ngtc); i++)
    // {
    //     fprintf(out, "  %10d", opts->anneal_npoints[i]);
    // }
    // fprintf(out, "\n");
    TextDumpComponent* comp = componentsStack.top();
    comp->addFormattedTextLeaf("%s%s", title.c_str(), bMDPformat ? " = " : ":");
    for (int i = 0; i < n; i++)
    {
        comp->printFormattedText("  %10d", vec[i]);
    }
}
    
void DumpStrategyText::pr_ivecs(const std::string title, const ivec vec[], int n)
{
}

void DumpStrategyText::pr_ivec_block(const std::string title, const int vec[], int n)
{
}

void DumpStrategyText::pr_rvec(const std::string title, const real vec[], int n) {
    if (available(vec, title))
    {
        pr_title_n(title, n);
        TextDumpComponent* comp = componentsStack.top();
        for (int i = 0; i < n; i++)
        {
            comp->addFormattedTextLeaf("%s[%d]=%12.5e", title.c_str(), i, vec[i]);
        }
        close_section();
    }
}

void DumpStrategyText::pr_rvec_row(const std::string title, const real vec[], int n) {
    if (available(vec, title))
    {
        TextDumpComponent* comp = componentsStack.top();
        comp->addFormattedTextLeaf("%s%s", title.c_str(), bMDPformat ? " = " : ":");
        for (int i = 0; i < n; i++)
        {
            comp->printFormattedText("  %10g", vec[i]);
        }
    }
}
void DumpStrategyText::pr_dvec_row(const std::string title, const double vec[], int n)
{
    if (available(vec, title))
    {
        TextDumpComponent* comp = componentsStack.top();
        comp->addFormattedTextLeaf("%s%s", title, bMDPformat ? " = " : ":");
        for (int i = 0; i < n; i++)
        {
            comp->printFormattedText("  %10g", vec[i]);
        }
    }
}

void DumpStrategyText::pr_svec_row(const std::string title, const char* vec[], int n)
{
    if (available(vec, title))
    {
        TextDumpComponent* comp = componentsStack.top();
        comp->addFormattedTextLeaf("%s%s", title, bMDPformat ? " = " : ":");
        for (int i = 0; i < n; i++)
        {
            comp->printFormattedText("  %10s", vec[i]);
        }
    }
}

void DumpStrategyText::pr_rvecs(const std::string title, const rvec vec[], int n)
{
    const std::string fshort = "%12.5e";
    const std::string flong  = "%15.8e";
    const std::string format = (getenv("GMX_PRINT_LONGFORMAT") != nullptr) ? flong : fshort;
    int         i, j;

    if (available(vec, title)) {
        pr_title_nxn(title, n, DIM);
        TextDumpComponent* comp = componentsStack.top();

        for (i = 0; i < n; i++) {
            comp->addFormattedTextLeaf("%s[%5d]=", title.c_str(), i);
            comp->addTextVectorLeaf(vec[i], DIM);
        }

        componentsStack.pop();
    }
}

void DumpStrategyText::pr_matrix(const std::string title, const rvec* m) {
    if (bMDPformat) {
        // fprintf(fp,
        //         "%-10s    = %g %g %g %g %g %g\n",
        //         title,
        //         m[XX][XX],
        //         m[YY][YY],
        //         m[ZZ][ZZ],
        //         m[XX][YY],
        //         m[XX][ZZ],
        //         m[YY][ZZ]);
    } else {
        pr_rvecs(title, m, DIM);
    }
}

void DumpStrategyText::pr_kvtree(const gmx::KeyValueTreeObject kvTree)
{
    for (const auto& prop : kvTree.properties())
    {
        const auto& value = prop.value();
        if (value.isObject())
        {
            DumpComponent::setIndent(2);
            pr_title(prop.key().c_str());
            pr_kvtree(value.asObject());
            close_section();
            DumpComponent::setIndent(3);
        }
        else if (value.isArray()
                 && std::all_of(value.asArray().values().begin(),
                                value.asArray().values().end(),
                                [](const auto& elem) { return elem.isObject(); }))
        {
            DumpComponent::setIndent(2);
            pr_title(prop.key().c_str());
            for (const auto& elem : value.asArray().values())
            {
                pr_kvtree(elem.asObject());
            }
            close_section();
            DumpComponent::setIndent(3);
        } else {
            if (value.isArray())
            {
                // TODO: implement
                // writer->writeString("[");
                // for (const auto& elem : value.asArray().values())
                // {
                //     GMX_RELEASE_ASSERT(
                //             !elem.isObject() && !elem.isArray(),
                //             "Only arrays of simple types and array of objects are implemented. "
                //             "Arrays of arrays and mixed arrays are not supported.");
                //     writer->writeString(" ");
                //     writer->writeString(simpleValueToString(elem));
                // }
                // writer->writeString(" ]");
            }
            else
            {
                int spacing = 33 - componentsStack.top()->getIndent();
                componentsStack.top()->addAlignedTextLeaf(prop.key().c_str(), simpleValueToString(value), spacing);
            }

        }
    }
}

void DumpStrategyText::pr_tpx_header(const TpxFileHeader* sh)
{
}

// Tutaj pokracuj
    
void DumpStrategyText::pr_sim_annealing(const std::string title, const SimulatedAnnealing sa[], int n)
{
    // fprintf(out, "annealing%s", bMDPformat ? " = " : ":");
    // for (i = 0; (i < opts->ngtc); i++)
    // {
    //     fprintf(out, "  %10s", enumValueToString(opts->annealing[i]));
    // }
    // fprintf(out, "\n");
    TextDumpComponent* comp = componentsStack.top();
    comp->addFormattedTextLeaf("%s%s", title.c_str(), bMDPformat ? " = " : ":");
    for (int i = 0; i < n; i++)
    {
        comp->printFormattedText("  %10s", enumValueToString(sa[i]));
    }
}

// Toto ostava
void DumpStrategyText::pr_grps(gmx::ArrayRef<const AtomGroupIndices> grps, const char* const* const* grpname)
{
    int index = 0;
    for (const auto& group : grps)
    {
        componentsStack.top()->printFormattedText(
            "\ngrp[%-12s] nr=%zu, name=[",
            shortName(static_cast<SimulationAtomGroupType>(index)),
            group.size()
        );
        for (const auto& entry : group)
        {
            componentsStack.top()->printFormattedText(" %s", *(grpname[entry]));
        }
        componentsStack.top()->printFormattedText("]");
        index++;
    }
}

// Toto ostava
void DumpStrategyText::pr_moltype(const int moltype, const std::string moltypeName) 
{
    componentsStack.top()->addFormattedTextLeaf(
        "%-20s = %d \"%s\"", "moltype", moltype, moltypeName.c_str()
    );
}

// Toto ostava
// TODO: prerobit na pr_group_stat
void DumpStrategyText::pr_group_stats(gmx::EnumerationArray<SimulationAtomGroupType, std::vector<int>>* gcount)
{
    componentsStack.top()->addGroupStats(gcount);
}

// Toto ostava
void DumpStrategyText::pr_anneal_points(const std::string title, const float vec[], int n)
{
    componentsStack.top()->addFormattedTextLeaf("%s [%d]:\t", title.c_str(), n);
    for (int i = 0; (i < vec[i]); i++)
    {
        componentsStack.top()->printFormattedText("  %10.1f", vec[i]);
    }
}

// Toto ostava
void DumpStrategyText::pr_functypes(const std::vector<int>& functype, const int n, const std::vector<t_iparams>& iparams)
{
    TextDumpComponent* comp = componentsStack.top()->addEmptySection();
    for (int i = 0; i < n; i++)
    {
        comp->addFormattedTextLeaf(
            "functype[%d]=%s",
            // bShowNumbers ? i : -1,
            i,
            interaction_function[functype[i]].name
        );
        std::vector<KeyFormatValue> kfvs = getInteractionParameters(functype[i], iparams[i]);
        for (size_t j = 0; j < kfvs.size(); j++) {
            comp->printFormattedText(", %s=", kfvs[j].key);
            if (std::holds_alternative<int>(kfvs[j].value)) {
                comp->printFormattedText(kfvs[j].format, std::get<int>(kfvs[j].value));
            } else if (std::holds_alternative<float>(kfvs[j].value)) {
                comp->printFormattedText(kfvs[j].format, std::get<float>(kfvs[j].value));
            } else if (std::holds_alternative<double>(kfvs[j].value)) {
                comp->printFormattedText(kfvs[j].format, std::get<double>(kfvs[j].value));
            } else if (std::holds_alternative<int64_t>(kfvs[j].value)) {
                comp->printFormattedText(kfvs[j].format, std::get<int64_t>(kfvs[j].value));
            } else if (std::holds_alternative<real>(kfvs[j].value)) {
                comp->printFormattedText(kfvs[j].format, std::get<real>(kfvs[j].value));
            } else if (std::holds_alternative<std::array<real, 3>>(kfvs[j].value)) {
                std::array<real, 3> arr = std::get<std::array<real, 3>>(kfvs[j].value);
                comp->printFormattedText("(");
                for (int k = 0; k < DIM - 1; k++)
                {
                    comp->printFormattedText(kfvs[j].format, arr[k]);
                    comp->printFormattedText(",");
                }
                comp->printFormattedText(kfvs[j].format, arr[DIM - 1]);
                comp->printFormattedText(")");
            } else {
                comp->printFormattedText("unknown_format");
            }
        }
        // pr_iparams(fp, ffparams->functype[i], ffparams->iparams[i]);
    //     pr_indent(fp, indent + INDENT);
    //     fprintf(fp,
    //             "functype[%d]=%s, ",
    //             bShowNumbers ? i : -1,
    //             interaction_function[ffparams->functype[i]].name);
    //     pr_iparams(fp, ffparams->functype[i], ffparams->iparams[i]);
    }
}

// Toto ostava
void DumpStrategyText::pr_grp_opt_agg(
    const rvec acceleration[], const int ngacc,
    const ivec nFreeze[], const int ngfrz,
    const int egp_flags[], const int ngener
)
{
    TextDumpComponent* comp = componentsStack.top()->addEmptySection();
    int i, m;

    // pr_indent(out, indent);
    // fprintf(out, "acc:\t");
    // for (i = 0; (i < opts->ngacc); i++)
    // {
    //     for (m = 0; (m < DIM); m++)
    //     {
    //         fprintf(out, "  %10g", opts->acceleration[i][m]);
    //     }
    // }
    // fprintf(out, "\n");
    comp->addFormattedTextLeaf("acc:\t");
    for (i = 0; i < ngacc; i++)
    {
        for (m = 0; m < DIM; m++)
        {
            comp->printFormattedText("  %10g", acceleration[i][m]);
        }
    }

    // pr_indent(out, indent);
    // fprintf(out, "nfreeze:");
    // for (i = 0; (i < opts->ngfrz); i++)
    // {
    //     for (m = 0; (m < DIM); m++)
    //     {
    //         fprintf(out, "  %10s", opts->nFreeze[i][m] ? "Y" : "N");
    //     }
    // }
    // fprintf(out, "\n");
    comp->addFormattedTextLeaf("nfreeze:");
    for (i = 0; i < ngfrz; i++)
    {
        for (m = 0; m < DIM; m++)
        {
            comp->printFormattedText("  %10s", nFreeze[i][m] ? "Y" : "N");
        }
    }

    // for (i = 0; (i < opts->ngener); i++)
    // {
    //     pr_indent(out, indent);
    //     fprintf(out, "energygrp-flags[%3d]:", i);
    //     for (m = 0; (m < opts->ngener); m++)
    //     {
    //         fprintf(out, " %d", opts->egp_flags[opts->ngener * i + m]);
    //     }
    //     fprintf(out, "\n");
    // }
    for (i = 0; (i < ngener); i++)
    {
        comp->addFormattedTextLeaf("energygrp-flags[%3d]:", i);
        for (m = 0; m < ngener; m++)
        {
            comp->printFormattedText(" %d", egp_flags[ngener * i + m]);
        }
    }
}

// TODO: divide between builder and strategy functions
// TODO: bShowNumbers to strategy as static member
void DumpStrategyText::pr_atoms(const t_atoms* atoms)
{
    bool bShowNumbers = true;
    pr_title("atoms");
    if (available(atoms->atom, "atom"))
    {
        pr_title_i("atom", atoms->nr);
        for (int i = 0; (i < atoms->nr); i++)
        {
            // fprintf(stderr,
            //         "atom[%6d]={type=%3hu, typeB=%3hu, ptype=%8s, m=%12.5e, "
            //         "q=%12.5e, mB=%12.5e, qB=%12.5e, resind=%5d, atomnumber=%3d}\n",
            //         i,
            //         atoms->atom[i].type,
            //         atoms->atom[i].typeB,
            //         enumValueToString(atoms->atom[i].ptype),
            //         atoms->atom[i].m,
            //         atoms->atom[i].q,
            //         atoms->atom[i].mB,
            //         atoms->atom[i].qB,
            //         atoms->atom[i].resind,
            //         atoms->atom[i].atomnumber);
            componentsStack.top()->addFormattedTextLeaf(
                    "atom[%6d]={type=%3hu, typeB=%3hu, ptype=%8s, m=%12.5e, "
                    "q=%12.5e, mB=%12.5e, qB=%12.5e, resind=%5d, atomnumber=%3d}",
                    i,
                    atoms->atom[i].type,
                    atoms->atom[i].typeB,
                    enumValueToString(atoms->atom[i].ptype),
                    atoms->atom[i].m,
                    atoms->atom[i].q,
                    atoms->atom[i].mB,
                    atoms->atom[i].qB,
                    atoms->atom[i].resind,
                    atoms->atom[i].atomnumber);
        }
        close_section();
        // pr_strings
        pr_title_i("atom", atoms->nr);
        for (int i = 0; (i < atoms->nr); i++)
        {
            componentsStack.top()->addFormattedTextLeaf(
                "%s[%d]={name=\"%s\"}",
                "atom", bShowNumbers ? i : -1, *(atoms->atomname[i]));
        }
        close_section();
        // pr_strings_2
        pr_title_i("type", atoms->nr);
        for (int i = 0; (i < atoms->nr); i++)
        {
            componentsStack.top()->addFormattedTextLeaf(
                "%s[%d]={name=\"%s\",nameB=\"%s\"}",
                "type", i, *(atoms->atomtype[i]), *(atoms->atomtypeB[i]));
        }
        close_section();
        pr_resinfo(atoms->resinfo, atoms->nres);
    }
    close_section();
}

void DumpStrategyText::pr_list_i(const std::string title, const int index, gmx::ArrayRef<const int> list)
{
    componentsStack.top()->printList(title, index, list);
}

void DumpStrategyText::pr_interaction_list(const std::string& title, const t_functype* functypes, const InteractionList& ilist, const t_iparams* iparams)
{
    // TODO: show numbers
    bool bShowNumbers = true;
    bool bShowParameters = false;

    pr_title(title.c_str());
    componentsStack.top()->addFormattedTextLeaf("nr: %d", ilist.size());

    if (ilist.empty())
    {
        close_section();
        return;
    }

    pr_title("iatoms");
    TextDumpComponent* comp = componentsStack.top();

    int j = 0;
    for (int i = 0; i < ilist.size();)
    {
        const int type  = ilist.iatoms[i];
        const int ftype = functypes[type];
        if (bShowNumbers)
        {
            comp->addFormattedTextLeaf("%d type=%d ", j, type);
        }
        j++;
        comp->printFormattedText("(%s)", interaction_function[ftype].name);
        for (int k = 0; k < interaction_function[ftype].nratoms; k++)
        {
            comp->printFormattedText(" %3d", ilist.iatoms[i + 1 + k]);
        }
        if (bShowParameters)
        {
            comp->printFormattedText("  ");
            // TODO: migrate to specific strategy func
            std::vector<KeyFormatValue> kfvs = getInteractionParameters(ftype, iparams[type]);
            for (size_t j = 0; j < kfvs.size(); j++) {
                comp->printFormattedText(", %s=", kfvs[j].key);
                if (std::holds_alternative<int>(kfvs[j].value)) {
                    comp->printFormattedText(kfvs[j].format, std::get<int>(kfvs[j].value));
                } else if (std::holds_alternative<float>(kfvs[j].value)) {
                    comp->printFormattedText(kfvs[j].format, std::get<float>(kfvs[j].value));
                } else if (std::holds_alternative<double>(kfvs[j].value)) {
                    comp->printFormattedText(kfvs[j].format, std::get<double>(kfvs[j].value));
                } else if (std::holds_alternative<int64_t>(kfvs[j].value)) {
                    comp->printFormattedText(kfvs[j].format, std::get<int64_t>(kfvs[j].value));
                } else if (std::holds_alternative<real>(kfvs[j].value)) {
                    comp->printFormattedText(kfvs[j].format, std::get<real>(kfvs[j].value));
                } else if (std::holds_alternative<std::array<real, 3>>(kfvs[j].value)) {
                    std::array<real, 3> arr = std::get<std::array<real, 3>>(kfvs[j].value);
                    comp->printFormattedText("( ");
                    for (int k = 0; k < DIM - 1; k++)
                    {
                        comp->printFormattedText(kfvs[j].format, arr[k]);
                        comp->printFormattedText(", ");
                    }
                    comp->printFormattedText(kfvs[j].format, arr[DIM - 1]);
                    comp->printFormattedText(")");
                } else {
                    std::array<real, 3> arr = std::get<std::array<real, 3>>(kfvs[j].value);
                    comp->printFormattedText("unknown_format");
                }
            }
        }
        i += 1 + interaction_function[ftype].nratoms;
    }

    close_section();
    close_section();

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

// Toto ostava
void DumpStrategyText::pr_groups(const SimulationGroups& groups)
{
    TextDumpComponent* comp = componentsStack.top();
    comp->addFormattedTextLeaf("groups          ");
    for (const auto group : gmx::EnumerationWrapper<SimulationAtomGroupType>{})
    {
        comp->printFormattedText(" %5.5s", shortName(group));
    }

    comp->addFormattedTextLeaf("allocated       ");
    int nat_max = 0;
    for (auto group : keysOf(groups.groups))
    {
        comp->printFormattedText(" %5d", groups.numberOfGroupNumbers(group));
        nat_max = std::max(nat_max, groups.numberOfGroupNumbers(group));
    }

    if (nat_max == 0)
    {
        comp->addFormattedTextLeaf("groupnr[%5s] =", "*");
        for (auto gmx_unused group : keysOf(groups.groups))
        {
            comp->printFormattedText("  %3d ", 0);
        }
    }
    else
    {
        for (int i = 0; i < nat_max; i++)
        {
            comp->addFormattedTextLeaf("groupnr[%5d] =", i);
            for (auto group : keysOf(groups.groups))
            {
                comp->printFormattedText("  %3d ",
                    !groups.groupNumbers[group].empty() ? groups.groupNumbers[group][i] : 0);
            }
        }
    }
}

// TODO: split between builder and pr_residue / pr_residues ?
void DumpStrategyText::pr_resinfo(const t_resinfo* resinfo, int n)
{
    bool bShowNumbers = true;
    if (!available(resinfo, "residue"))
    {
        return;
    }

    pr_title_n("residue", n);
    TextDumpComponent* comp = componentsStack.top();
    for (int i = 0; i < n; i++)
    {
        comp->addFormattedTextLeaf(
            "residue[%d]={name=\"%s\", nr=%d, ic='%c'}",
            bShowNumbers ? i : -1,
            *(resinfo[i].name),
            resinfo[i].nr,
            (resinfo[i].ic == '\0') ? ' ' : resinfo[i].ic
        );
    }
    close_section();
}

// Toto ostava
void DumpStrategyText::pr_cmap(const gmx_cmap_t* cmap_grid)
{ 
    // TODO: bShowNumbers 
    bool bShowNumbers = true;
    const real dx = cmap_grid->grid_spacing != 0 ? 360.0 / cmap_grid->grid_spacing : 0;

    const int nelem = cmap_grid->grid_spacing * cmap_grid->grid_spacing;

    if (!available(cmap_grid, "cmap"))
    {
        return;
    }

    TextDumpComponent* comp = componentsStack.top();
    comp->printFormattedText("\ncmap");

    for (gmx::Index i = 0; i < gmx::ssize(cmap_grid->cmapdata); i++)
    {
        real idx = -180.0;
        comp->printFormattedText("\n%8s %8s %8s %8s", "V", "dVdx", "dVdy", "d2dV");

        comp->printFormattedText("\ngrid[%3zd]={", bShowNumbers ? i : -1);

        for (int j = 0; j < nelem; j++)
        {
            if ((j % cmap_grid->grid_spacing) == 0)
            {
                comp->printFormattedText("\n%8.1f", idx);
                idx += dx;
            }

            comp->printFormattedText("\n%8.3f ", cmap_grid->cmapdata[i].cmap[j * 4]);
            comp->printFormattedText("%8.3f ", cmap_grid->cmapdata[i].cmap[j * 4 + 1]);
            comp->printFormattedText("%8.3f ", cmap_grid->cmapdata[i].cmap[j * 4 + 2]);
            comp->printFormattedText("%8.3f", cmap_grid->cmapdata[i].cmap[j * 4 + 3]);
        }
        comp->printFormattedText("\n");
    }
}

// void DumpStrategyText::pr_reals(const std::string title, const real* vec, int n)
// {
//     // if (available(fp, vec, indent, title))
//     // {
//     //     pr_indent(fp, indent);
//     //     fprintf(fp, "%s:\t", title);
//     //     for (int i = 0; i < n; i++)
//     //     {
//     //         fprintf(fp, "  %10g", vec[i]);
//     //     }
//     //     fprintf(fp, "\n");
//     // }
// }

// void DumpStrategyText::pr_doubles(const std::string title, const double* vec, int n)
// {
//     // if (available(fp, vec, indent, title))
//     // {
//     //     pr_indent(fp, indent);
//     //     fprintf(fp, "%s:\t", title);
//     //     for (int i = 0; i < n; i++)
//     //     {
//     //         fprintf(fp, "  %10g", vec[i]);
//     //     }
//     //     fprintf(fp, "\n");
//     // }
// }

// void DumpStrategyText::pr_reals_of_dim(const std::string title, const real* vec, int n, int dim)
// {
//     // const std::string fshort = "%12.5e";
//     // const std::string flong  = "%15.8e";
//     // const std::string format = (getenv("GMX_PRINT_LONGFORMAT") != nullptr) ? flong : fshort;

//     // if (available(fp, vec, indent, title))
//     // {
//     //     indent = pr_title_nxn(fp, indent, title, n, dim);
//     //     for (int i = 0; i < n; i++)
//     //     {
//     //         pr_indent(fp, indent);
//     //         fprintf(fp, "%s[%5d]={", title, i);
//     //         for (int j = 0; j < dim; j++)
//     //         {
//     //             if (j != 0)
//     //             {
//     //                 fprintf(fp, ", ");
//     //             }
//     //             fprintf(fp, format, vec[i * dim + j]);
//     //         }
//     //         fprintf(fp, "}\n");
//     //     }
//     // }
// }

// void DumpStrategyText::pr_int(const std::string title, int i)
// {
//     // pr_indent(fp, indent);
//     // fprintf(fp, "%-30s = %d\n", title, i);
// }

// void DumpStrategyText::pr_int64(const std::string title, int64_t i)
// {
//     // char buf[STEPSTRSIZE];

//     // pr_indent(fp, indent);
//     // fprintf(fp, "%-30s = %s\n", title, gmx_step_str(i, buf));
// }

// void DumpStrategyText::pr_real(const std::string title, real r)
// {
//     // pr_indent(fp, indent);
//     // fprintf(fp, "%-30s = %g\n", title, r);
// }

// void DumpStrategyText::pr_double(const std::string title, double d)
// {
//     // pr_indent(fp, indent);
//     // fprintf(fp, "%-30s = %g\n", title, d);
// }

// void DumpStrategyText::pr_str(const std::string title, const char* s)
// {
//     // pr_indent(fp, indent);
//     // fprintf(fp, "%-30s = %s\n", title, s);
//     componentsStack.top()->addFormattedTextLeaf("%-30s = %s\n", title, s);
// }

// void DumpStrategyText::pr_strings(const std::string title, const char* const* const* nm, int n, gmx_bool bShowNumbers)
// {
//     // if (available(fp, nm, indent, title))
//     // {
//     //     indent = pr_title_n(fp, indent, title, n);
//     //     for (int i = 0; i < n; i++)
//     //     {
//     //         pr_indent(fp, indent);
//     //         fprintf(fp, "%s[%d]={name=\"%s\"}\n", title, bShowNumbers ? i : -1, *(nm[i]));
//     //     }
//     // }
// }
