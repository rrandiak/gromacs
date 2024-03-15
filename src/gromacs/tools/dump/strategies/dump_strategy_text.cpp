#include "gromacs/tools/dump/strategies/dump_strategy_text.h"

int DumpComponent::indentValue = 3;

DumpStrategyText::DumpStrategyText(FILE* fp)
{
    TextDumpComponent* root = new TextRootComponent(fp);
    componentsStack.push(root);
}

bool DumpStrategyText::available(const void* p, const char* title) {
    if (!p) {
        componentsStack.top()->addTextLeaf(title, "not available");
    }
    return (p != nullptr);
}

void DumpStrategyText::pr_filename(const char* filename) {
    componentsStack.top()->printFilename(filename);
}

void DumpStrategyText::pr_title(const char* title) {
    TextDumpComponent* comp = componentsStack.top()->addTextObject(title);
    componentsStack.push(comp);
}

void DumpStrategyText::pr_title_i(const char* title, int i) {
    // fprintf(fp, "%s (%d):\n", title, n);
}

void DumpStrategyText::pr_title_n(const char* title, int n) {
    int bufferSize = snprintf(nullptr, 0, "%s (%d)", title, n) + 1;
    std::string formattedString(bufferSize, '\0');
    snprintf(&formattedString[0], bufferSize, "%s (%d)", title, n);

    TextDumpComponent* comp = componentsStack.top()->addTextObject(formattedString.c_str());
    componentsStack.push(comp);
}

void DumpStrategyText::pr_title_nxn(const char* title, int n1, int n2) {
    int bufferSize = snprintf(nullptr, 0, "%s (%dx%d)", title, n1, n2) + 1;
    std::string formattedString(bufferSize, '\0');
    snprintf(&formattedString[0], bufferSize, "%s (%dx%d)", title, n1, n2);

    TextDumpComponent* comp = componentsStack.top()->addTextObject(formattedString.c_str());
    componentsStack.push(comp);
}

void DumpStrategyText::close_section() {
    componentsStack.pop();
}
    
void DumpStrategyText::pr_is_present(const char* title, gmx_bool b) {
    componentsStack.top()->addAlignedTextLeaf(title, b ? "present" : "not present", 6);
}

void DumpStrategyText::pr_named_value(const char* name, const Value& value) {
    componentsStack.top()->addTextLeaf(name, value);
}

void DumpStrategyText::pr_attribute(const char* name, const Value& value)
{
    componentsStack.top()->addAttribute(name, value);
}

void DumpStrategyText::pr_name(const char* name) {
    componentsStack.top()->addFormattedTextLeaf("name=\"%s\"", name);
}

void DumpStrategyText::pr_matrix(const char* title, const rvec* m, gmx_bool bMDPformat) {
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

void DumpStrategyText::pr_sim_annealing(const char* title, const SimulatedAnnealing sa[], int n, gmx_bool bMDPformat)
{
    // fprintf(out, "annealing%s", bMDPformat ? " = " : ":");
    // for (i = 0; (i < opts->ngtc); i++)
    // {
    //     fprintf(out, "  %10s", enumValueToString(opts->annealing[i]));
    // }
    // fprintf(out, "\n");
    TextDumpComponent* comp = componentsStack.top();
    comp->addFormattedTextLeaf("%s%s", title, bMDPformat ? " = " : ":");
    for (int i = 0; i < n; i++)
    {
        comp->printFormattedText("  %10s", enumValueToString(sa[i]));
    }
}

void DumpStrategyText::pr_vec_row(const char* title, const Value vec[], int n, gmx_bool bShowNumbers)
{
}

void DumpStrategyText::pr_rvec(const char* title, const real vec[], int n, gmx_bool bShowNumbers) {
    if (available(vec, title))
    {
        pr_title_n(title, n);
        TextDumpComponent* comp = componentsStack.top();
        for (int i = 0; i < n; i++)
        {
            comp->addFormattedTextLeaf("%s[%d]=%12.5e", title, i, vec[i]);
        }
        close_section();
    }
}

void DumpStrategyText::pr_rvec_row(const char* title, const real vec[], int n, gmx_bool bMDPformat) {
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

void DumpStrategyText::pr_rvecs(const char* title, const rvec vec[], int n)
{
    const char* fshort = "%12.5e";
    const char* flong  = "%15.8e";
    const char* format = (getenv("GMX_PRINT_LONGFORMAT") != nullptr) ? flong : fshort;
    int         i, j;

    if (available(vec, title)) {
        pr_title_nxn(title, n, DIM);
        TextDumpComponent* comp = componentsStack.top();

        for (i = 0; i < n; i++) {
            comp->addFormattedTextLeaf("%s[%5d]=", title, i);
            comp->addTextVectorLeaf(vec[i], DIM);
            // fprintf(fp, "%s[%5d]={", title, i);
            // for (j = 0; j < DIM; j++)
            // {
            //     if (j != 0)
            //     {
            //         fprintf(fp, ", ");
            //     }
            //     fprintf(fp, format, vec[i][j]);
            // }
            // fprintf(fp, "}\n");
        }

        componentsStack.pop();
    }
}

void DumpStrategyText::pr_ivec_row(const char* title, const int vec[], int n, gmx_bool bMDPformat)
{
    // fprintf(out, "annealing-npoints%s", bMDPformat ? " = " : ":");
    // for (i = 0; (i < opts->ngtc); i++)
    // {
    //     fprintf(out, "  %10d", opts->anneal_npoints[i]);
    // }
    // fprintf(out, "\n");
    TextDumpComponent* comp = componentsStack.top();
    comp->addFormattedTextLeaf("%s%s", title, bMDPformat ? " = " : ":");
    for (int i = 0; i < n; i++)
    {
        comp->printFormattedText("  %10d", vec[i]);
    }
}

void DumpStrategyText::pr_ivec(const char* title, const int vec[], int n, gmx_bool bShowNumbers)
{
}
    
void DumpStrategyText::pr_ivecs(const char* title, const ivec vec[], int n)
{
}

void DumpStrategyText::pr_ivec_block(const char* title, const int vec[], int n, gmx_bool bShowNumbers)
{
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

// void DumpStrategyText::pr_group_nr()
// {
// }

void DumpStrategyText::pr_grps(gmx::ArrayRef<const AtomGroupIndices> grps, const char* const* const* grpname)
{
    int index = 0;
    for (const auto& group : grps)
    {
        componentsStack.top()->addFormattedTextLeaf(
            "%s[%-12s] nr=%zu, name=[",
            "grp",
            shortName(static_cast<SimulationAtomGroupType>(index)),
            group.size()
        );
        for (const auto& entry : group)
        {
            componentsStack.top()->addFormattedTextLeaf(" %s", *(grpname[entry]));
        }
        componentsStack.top()->addFormattedTextLeaf("]\n");
        index++;
    }
    // int index = 0;
    // for (const auto& group : grps)
    // {
    //     fprintf(fp,
    //             "%s[%-12s] nr=%zu, name=[",
    //             title,
    //             shortName(static_cast<SimulationAtomGroupType>(index)),
    //             group.size());
    //     for (const auto& entry : group)
    //     {
    //         fprintf(fp, " %s", *(grpname[entry]));
    //     }
    //     fprintf(fp, "]\n");
    //     index++;
    // }
}

void DumpStrategyText::pr_moltype(const int moltype, const char* moltypeName) 
{
    componentsStack.top()->addFormattedTextLeaf(
        "%-20s = %d \"%s\"", "moltype", moltype, moltypeName
    );
}

void DumpStrategyText::pr_group_stats(gmx::EnumerationArray<SimulationAtomGroupType, std::vector<int>>* gcount)
{
    componentsStack.top()->addGroupStats(gcount);
}

void DumpStrategyText::pr_anneal_points(const char* title, const float vec[], int n)
{
    componentsStack.top()->addFormattedTextLeaf("%s [%d]:\t", title, n);
    for (int i = 0; (i < vec[i]); i++)
    {
        componentsStack.top()->printFormattedText("  %10.1f", vec[i]);
    }
}

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


// void DumpStrategyText::pr_reals(const char* title, const real* vec, int n)
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

// void DumpStrategyText::pr_doubles(const char* title, const double* vec, int n)
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

// void DumpStrategyText::pr_reals_of_dim(const char* title, const real* vec, int n, int dim)
// {
//     // const char* fshort = "%12.5e";
//     // const char* flong  = "%15.8e";
//     // const char* format = (getenv("GMX_PRINT_LONGFORMAT") != nullptr) ? flong : fshort;

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

// void DumpStrategyText::pr_int(const char* title, int i)
// {
//     // pr_indent(fp, indent);
//     // fprintf(fp, "%-30s = %d\n", title, i);
// }

// void DumpStrategyText::pr_int64(const char* title, int64_t i)
// {
//     // char buf[STEPSTRSIZE];

//     // pr_indent(fp, indent);
//     // fprintf(fp, "%-30s = %s\n", title, gmx_step_str(i, buf));
// }

// void DumpStrategyText::pr_real(const char* title, real r)
// {
//     // pr_indent(fp, indent);
//     // fprintf(fp, "%-30s = %g\n", title, r);
// }

// void DumpStrategyText::pr_double(const char* title, double d)
// {
//     // pr_indent(fp, indent);
//     // fprintf(fp, "%-30s = %g\n", title, d);
// }

// void DumpStrategyText::pr_str(const char* title, const char* s)
// {
//     // pr_indent(fp, indent);
//     // fprintf(fp, "%-30s = %s\n", title, s);
//     componentsStack.top()->addFormattedTextLeaf("%-30s = %s\n", title, s);
// }

// void DumpStrategyText::pr_strings(const char* title, const char* const* const* nm, int n, gmx_bool bShowNumbers)
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
