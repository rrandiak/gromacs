#include "text_strategy.h"
#include "gromacs/tools/dump/components/dump_components_text.h"
#include "gromacs/tools/dump/dump_component.h"
#include "gromacs/topology/ifunc.h"
#include "gromacs/utility/stringutil.h"

TextStrategy::TextStrategy(FILE* fp)
{
    TextComponent* root = new TextRootComponent(fp);
    componentsStack.push(root);
}

TextStrategy::~TextStrategy()
{
    while (componentsStack.size() > 1)
    {
        componentsStack.pop();
    }
    if (!componentsStack.empty())
    {
        TextComponent* comp = componentsStack.top();
        componentsStack.pop();
        delete comp;
    }
}

bool TextStrategy::available(const void* p, const std::string title)
{
    if (!p) {
        componentsStack.top()->addFormattedTextLeaf("%s: %s", title.c_str(), "not available");
    }
    return (p != nullptr);
}

void TextStrategy::pr_filename(const std::string filename)
{
    componentsStack.top()->printFormattedText("%s:", filename.c_str());
}

void TextStrategy::pr_title(const std::string title)
{
    TextComponent* comp = componentsStack.top()->addSection(title);
    componentsStack.push(comp);
}

void TextStrategy::pr_title_i(const std::string title, int i)
{
    TextComponent* comp = componentsStack.top()->addSection(
        gmx::formatString("%s (%d)", title.c_str(), i)
    );
    componentsStack.push(comp);
}

void TextStrategy::pr_title_n(const std::string title, int n)
{
    pr_title_i(title, n);
}

void TextStrategy::pr_title_nxm(const std::string title, int n, int m)
{
    TextComponent* comp = componentsStack.top()->addSection(
        gmx::formatString("%s (%dx%d)", title.c_str(), n, m)
    );
    componentsStack.push(comp);
}

void TextStrategy::pr_title_list(const std::string title)
{
    // Intetionally left blank
}

void TextStrategy::pr_title_atom_names(int n)
{
    pr_title_n("atom", n);
}

void TextStrategy::pr_title_pull_group(std::string title, int i)
{
    TextComponent* comp = componentsStack.top()->addSection(gmx::formatString("%s %d", title.c_str(), i), 2);
    componentsStack.push(comp);
}

void TextStrategy::pr_title_pull_coord(std::string title, int i)
{
    TextComponent* comp = componentsStack.top()->addSection(gmx::formatString("%s %d", title.c_str(), i), 0);
    componentsStack.push(comp);
}

void TextStrategy::pr_title_rot_group(std::string title, int i)
{
    TextComponent* comp = componentsStack.top()->addSection(gmx::formatString("%s %d", title.c_str(), i), 2);
    componentsStack.push(comp);
}

void TextStrategy::pr_title_awh(std::string title)
{
    TextComponent* comp = componentsStack.top()->addSection(title, 1);
    componentsStack.push(comp);
}

void TextStrategy::pr_title_all_lambdas(std::string title)
{
    TextComponent* comp = componentsStack.top()->addSectionWithIndent(title, 0);
    componentsStack.push(comp);
}

void TextStrategy::close_section()
{
    componentsStack.pop();
}

void TextStrategy::close_list()
{
    // Intetionally left blank
}

void TextStrategy::pr_named_value(const std::string name, const Value& value)
{
    componentsStack.top()->printLeaf(name, value, 30);
}

void TextStrategy::pr_named_value_short_format(const std::string name, const Value& value)
{
    componentsStack.top()->printLeaf(name, value, 6);
}

void TextStrategy::pr_named_value_scientific(const std::string name, const real& value)
{
    componentsStack.top()->addFormattedTextLeaf("%-*s = %e", 6, name.c_str(), value);
}

void TextStrategy::pr_count(const std::string name, const Value& value)
{
    pr_named_value(gmx::formatString("#%s", name.c_str()), value);
}

void TextStrategy::pr_attribute(const std::string name, const Value& value)
{
    componentsStack.top()->printAttribute(name, value);
}

void TextStrategy::pr_attribute_quoted(const std::string name, const std::string& value)
{
    pr_attribute(name, "\"" + value + "\"");
}
    
void TextStrategy::pr_vec_attributes(const std::string title, int i, const char** names, char** values, int n)
{
    componentsStack.top()->addFormattedTextLeaf(
        "%s[%d]={", title.c_str(), bShowNumbers ? i : -1
    );
    for (int j = 0; j < n - 1; j++)
    {
        componentsStack.top()->printFormattedText("%s=\"%s\",", names[j], values[j]);
    }
    componentsStack.top()->printFormattedText("%s=\"%s\"}", names[n - 1], values[n - 1]);
}

void TextStrategy::pr_residue(const t_resinfo* resinfo, int i)
{
    componentsStack.top()->addFormattedTextLeaf(
        "residue[%d]={name=\"%s\", nr=%d, ic='%c'}",
        bShowNumbers ? i : -1,
        *(resinfo->name),
        resinfo->nr,
        (resinfo->ic == '\0') ? ' ' : resinfo->ic
    );
}

void TextStrategy::pr_ivec(const std::string title, const int vec[], int n)
{
    if (available(vec, title))
    {
        pr_title_n(title, n);
        TextComponent* comp = componentsStack.top();
        for (int i = 0; i < n; i++)
        {
            comp->addFormattedTextLeaf("%s[%d]=%d", title.c_str(), i, vec[i]);
        }
        close_section();
    }
}

void TextStrategy::pr_rvec(const std::string title, const real vec[], int n) {
    if (available(vec, title))
    {
        pr_title_n(title, n);
        TextComponent* comp = componentsStack.top();
        for (int i = 0; i < n; i++)
        {
            comp->addFormattedTextLeaf("%s[%d]=%12.5e", title.c_str(), i, vec[i]);
        }
        close_section();
    }
}
    
void TextStrategy::pr_ivecs(const std::string title, const ivec vec[], int n)
{
    int i, j;

    if (available(vec, title))
    {
        pr_title_nxm(title, n, DIM);
        TextComponent* comp = componentsStack.top();

        for (i = 0; i < n; i++)
        {
            comp->addFormattedTextLeaf("%s[%5d]={", title.c_str(), i);
            for (j = 0; j < DIM - 1; j++)
            {
                comp->printFormattedText("%d", vec[i][j]);
                comp->printFormattedText(", ");
            }
            comp->printFormattedText("%d", vec[i][DIM - 1]);
            comp->printFormattedText("}");
        }

        componentsStack.pop();
    }
}

void TextStrategy::pr_rvecs(const std::string title, const rvec vec[], int n)
{
    const std::string fshort = "%12.5e";
    const std::string flong  = "%15.8e";
    const std::string format = (getenv("GMX_PRINT_LONGFORMAT") != nullptr) ? flong : fshort;
    int i, j;

    if (available(vec, title))
    {
        pr_title_nxm(title, n, DIM);
        TextComponent* comp = componentsStack.top();

        for (i = 0; i < n; i++)
        {
            comp->addFormattedTextLeaf("%s[%5d]={", title.c_str(), i);
            for (j = 0; j < DIM - 1; j++)
            {
                comp->printFormattedText(format.c_str(), vec[i][j]);
                comp->printFormattedText(", ");
            }
            comp->printFormattedText(format.c_str(), vec[i][DIM - 1]);
            comp->printFormattedText("}");
        }

        componentsStack.pop();
    }
}

void TextStrategy::pr_ivec_row(const std::string title, const int vec[], int n)
{
    TextComponent* comp = componentsStack.top();
    comp->addFormattedTextLeaf("%s%s", title.c_str(), bMDPformat ? " = " : ":");
    for (int i = 0; i < n; i++)
    {
        comp->printFormattedText("  %10d", vec[i]);
    }
}

void TextStrategy::pr_rvec_row(const std::string title, const real vec[], int n) {
    if (available(vec, title))
    {
        TextComponent* comp = componentsStack.top();
        comp->addFormattedTextLeaf("%s%s", title.c_str(), bMDPformat ? " = " : ":");
        for (int i = 0; i < n; i++)
        {
            comp->printFormattedText("  %10g", vec[i]);
        }
    }
}
void TextStrategy::pr_dvec_row(const std::string title, const double vec[], int n)
{
    if (available(vec, title))
    {
        TextComponent* comp = componentsStack.top();
        comp->addFormattedTextLeaf("%s%s", title, bMDPformat ? " = " : ":");
        for (int i = 0; i < n; i++)
        {
            comp->printFormattedText("  %10g", vec[i]);
        }
    }
}

void TextStrategy::pr_svec_row(const std::string title, const char* vec[], int n)
{
    if (available(vec, title))
    {
        TextComponent* comp = componentsStack.top();
        comp->addFormattedTextLeaf("%s%s", title, bMDPformat ? " = " : ":");
        for (int i = 0; i < n; i++)
        {
            comp->printFormattedText("  %10s", vec[i]);
        }
    }
}
    
void TextStrategy::pr_sa_vec_row(const std::string title, const SimulatedAnnealing sa[], int n)
{
    TextComponent* comp = componentsStack.top();
    comp->addFormattedTextLeaf("%s%s", title.c_str(), bMDPformat ? " = " : ":");
    for (int i = 0; i < n; i++)
    {
        comp->printFormattedText("  %10s", enumValueToString(sa[i]));
    }
}

void TextStrategy::pr_ap_vec_row(const std::string title, const float vec[], int n, int index)
{
    componentsStack.top()->addFormattedTextLeaf("%s [%d]:\t", title.c_str(), index);
    for (int i = 0; (i < n); i++)
    {
        componentsStack.top()->printFormattedText("  %10.1f", vec[i]);
    }
}
    
void TextStrategy::pr_posrec_vec_row(const std::string title, const real vec[])
{
    TextComponent* comp = componentsStack.top();
    comp->addFormattedTextLeaf("%-11s", title.c_str());
    comp->printFormattedText("%g %g %g", vec[XX], vec[YY], vec[ZZ]);
}

void TextStrategy::pr_ivec_block(const std::string title, const int vec[], int n)
{
    int i, j;

    if (available(vec, title))
    {
        pr_title_n(title, n);
        i      = 0;
        while (i < n)
        {
            j = i + 1;
            while (j < n && vec[j] == vec[j - 1] + 1)
            {
                j++;
            }
            /* Print consecutive groups of 3 or more as blocks */
            if (j - i < 3)
            {
                while (i < j)
                {
                    componentsStack.top()->addFormattedTextLeaf( "%s[%d]=%d", title.c_str(), bShowNumbers ? i : -1, vec[i]);
                    i++;
                }
            }
            else
            {
                componentsStack.top()->addFormattedTextLeaf(
                        "%s[%d,...,%d] = {%d,...,%d}",
                        title.c_str(),
                        bShowNumbers ? i : -1,
                        bShowNumbers ? j - 1 : -1,
                        vec[i],
                        vec[j - 1]);
                i = j;
            }
        }
        close_section();
    }
}

void TextStrategy::pr_matrix(const std::string title, const rvec* m) {
    if (bMDPformat)
    {
        TextComponent* comp = componentsStack.top();
        comp->addFormattedTextLeaf("%-10s   ", title);
        comp->printFormattedText("%g %g %g %g %g %g\n",
            m[XX][XX], m[YY][YY], m[ZZ][ZZ],
            m[XX][YY], m[XX][ZZ], m[YY][ZZ]
        );
    }
    else
    {
        pr_rvecs(title, m, DIM);
    }
}

void TextStrategy::pr_kvtree(const gmx::KeyValueTreeObject kvTree)
{
    for (const auto& prop : kvTree.properties())
    {
        const auto& value = prop.value();
        if (value.isObject())
        {
            TextComponent* comp = componentsStack.top()->addSection(prop.key().c_str(), 2);
            componentsStack.push(comp);
            // pr_title(prop.key().c_str());
            pr_kvtree(value.asObject());
            close_section();
        }
        else if (value.isArray()
                 && std::all_of(value.asArray().values().begin(),
                                value.asArray().values().end(),
                                [](const auto& elem) { return elem.isObject(); }))
        {
            // pr_title(prop.key().c_str());
            TextComponent* comp = componentsStack.top()->addSection(prop.key().c_str(), 2);
            componentsStack.push(comp);
            for (const auto& elem : value.asArray().values())
            {
                pr_kvtree(elem.asObject());
            }
            close_section();
        }
        else
        {
            if (value.isArray())
            {
                componentsStack.top()->printFormattedText("[");
                for (const auto& elem : value.asArray().values())
                {
                    GMX_RELEASE_ASSERT(
                            !elem.isObject() && !elem.isArray(),
                            "Only arrays of simple types and array of objects are implemented. "
                            "Arrays of arrays and mixed arrays are not supported.");
                    componentsStack.top()->printFormattedText(" ");
                    componentsStack.top()->printFormattedText(simpleValueToString(elem).c_str());
                }
                componentsStack.top()->printFormattedText(" ]");
            }
            else
            {
                int spacing = 33 - componentsStack.top()->getIndent();
                componentsStack.top()->printLeaf(prop.key().c_str(), simpleValueToString(value), spacing);
            }

        }
    }
}

void TextStrategy::pr_moltype(const int moltype, const std::string moltypeName) 
{
    componentsStack.top()->addFormattedTextLeaf(
        "%-20s = %d \"%s\"", "moltype", moltype, moltypeName.c_str()
    );
}

void TextStrategy::pr_atom(const t_atom* atom, const int i)
{
    componentsStack.top()->addFormattedTextLeaf(
            "atom[%6d]={type=%3hu, typeB=%3hu, ptype=%8s, m=%12.5e, "
            "q=%12.5e, mB=%12.5e, qB=%12.5e, resind=%5d, atomnumber=%3d}",
            i,
            atom->type,
            atom->typeB,
            enumValueToString(atom->ptype),
            atom->m,
            atom->q,
            atom->mB,
            atom->qB,
            atom->resind,
            atom->atomnumber);
}

void TextStrategy::pr_grps(gmx::ArrayRef<const AtomGroupIndices> grps, const char* const* const* grpname)
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

void TextStrategy::pr_grp_opt_agg(
    const rvec acceleration[], const int ngacc,
    const ivec nFreeze[], const int ngfrz,
    const int egp_flags[], const int ngener
)
{
    TextComponent* comp = componentsStack.top()->addEmptySection();
    int i, m;

    comp->addFormattedTextLeaf("acc:\t");
    for (i = 0; i < ngacc; i++)
    {
        for (m = 0; m < DIM; m++)
        {
            comp->printFormattedText("  %10g", acceleration[i][m]);
        }
    }

    comp->addFormattedTextLeaf("nfreeze:");
    for (i = 0; i < ngfrz; i++)
    {
        for (m = 0; m < DIM; m++)
        {
            comp->printFormattedText("  %10s", nFreeze[i][m] ? "Y" : "N");
        }
    }

    for (i = 0; (i < ngener); i++)
    {
        comp->addFormattedTextLeaf("energygrp-flags[%3d]:", i);
        for (m = 0; m < ngener; m++)
        {
            comp->printFormattedText(" %d", egp_flags[ngener * i + m]);
        }
    }
}

void TextStrategy::pr_groups(const SimulationGroups& groups)
{
    TextComponent* comp = componentsStack.top();
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

void TextStrategy::pr_group_stats(gmx::EnumerationArray<SimulationAtomGroupType, std::vector<int>>* gcount)
{
    TextComponent* comp = componentsStack.top();
    int atot;

    comp->printFormattedText("\nGroup statistics\n");
    for (auto group : keysOf(*gcount))
    {
        atot = 0;
        comp->printFormattedText("%-12s: ", shortName(group));
        for (const auto& entry : (*gcount)[group])
        {
            comp->printFormattedText("  %5d", entry);
            atot += entry;
        }
        comp->printFormattedText("  (total %d atoms)\n", atot);
    }
}

void TextStrategy::pr_list_i(const std::string title, const int index, gmx::ArrayRef<const int> list)
{
    TextComponent* comp = componentsStack.top();

    if (list.empty())
    {
        comp->addFormattedTextLeaf("%s[%d]={}", title.c_str(), index);
        return;
    }

    comp->printList(
        gmx::formatString("%s[%d][num=%zu]", title.c_str(), bShowNumbers ? index : -1, list.size()),
        list
    );
}

void TextStrategy::pr_iparams(t_functype ftype, const t_iparams& iparams)
{
    TextComponent* comp = componentsStack.top();
    std::vector<KeyFormatValue> kfvs = getInteractionParameters(ftype, iparams);

    if (kfvs.empty())
    {
        comp->printFormattedText("\n");
        return;
    }

    for (size_t j = 0; j < kfvs.size(); j++)
    {
        // Temporary hack to make line break in text format
        if (kfvs[j].key == nullptr)
        {
            comp->printFormattedText("\n%s=", kfvs[++j].key);
        }
        else
        {
            comp->printFormattedText(", %s=", kfvs[j].key);
        }

        if (std::holds_alternative<int>(kfvs[j].value))
        {
            comp->printFormattedText(kfvs[j].format, std::get<int>(kfvs[j].value));
        }
        else if (std::holds_alternative<float>(kfvs[j].value))
        {
            comp->printFormattedText(kfvs[j].format, std::get<float>(kfvs[j].value));
        }
        else if (std::holds_alternative<double>(kfvs[j].value))
        {
            comp->printFormattedText(kfvs[j].format, std::get<double>(kfvs[j].value));
        }
        else if (std::holds_alternative<int64_t>(kfvs[j].value))
        {
            comp->printFormattedText(kfvs[j].format, std::get<int64_t>(kfvs[j].value));
        }
        else if (std::holds_alternative<real>(kfvs[j].value))
        {
            comp->printFormattedText(kfvs[j].format, std::get<real>(kfvs[j].value));
        }
        else if (std::holds_alternative<std::array<real, 3>>(kfvs[j].value))
        {
            std::array<real, 3> arr = std::get<std::array<real, 3>>(kfvs[j].value);
            comp->printFormattedText("(");
            for (int k = 0; k < DIM - 1; k++)
            {
                comp->printFormattedText(kfvs[j].format, arr[k]);
                comp->printFormattedText(",");
            }
            comp->printFormattedText(kfvs[j].format, arr[DIM - 1]);
            comp->printFormattedText(")");
        }
    }
}

void TextStrategy::pr_functypes(const std::vector<int>& functype, const int n, const std::vector<t_iparams>& iparams)
{
    TextComponent* comp = componentsStack.top()->addEmptySection();
    for (int i = 0; i < n; i++)
    {
        comp->addFormattedTextLeaf(
            "functype[%d]=%s",
            bShowNumbers ? i : -1,
            interaction_function[functype[i]].name
        );
        pr_iparams(functype[i], iparams[i]);
    }
}

void TextStrategy::pr_interaction_list(const std::string& title, const t_functype* functypes, const InteractionList& ilist, const t_iparams* iparams)
{
    pr_title(title.c_str());
    componentsStack.top()->addFormattedTextLeaf("nr: %d", ilist.size());

    if (ilist.empty())
    {
        close_section();
        return;
    }

    pr_title("iatoms");
    TextComponent* comp = componentsStack.top();

    int j = 0;
    for (int i = 0; i < ilist.size();)
    {
        const int type  = ilist.iatoms[i];
        const int ftype = functypes[type];
        if (bShowNumbers)
        {
            comp->addFormattedTextLeaf("%d type=%d ", j, type);
        }
        else
        {
            comp->addFormattedTextLeaf("");
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
            pr_iparams(ftype, iparams[type]);
        }
        i += 1 + interaction_function[ftype].nratoms;
    }

    close_section();
    close_section();
}

void TextStrategy::pr_cmap(const gmx_cmap_t* cmap_grid)
{ 
    const real dx = cmap_grid->grid_spacing != 0 ? 360.0 / cmap_grid->grid_spacing : 0;

    const int nelem = cmap_grid->grid_spacing * cmap_grid->grid_spacing;

    if (!available(cmap_grid, "cmap"))
    {
        return;
    }

    TextComponent* comp = componentsStack.top();
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

void TextStrategy::pr_separate_dvdl(const std::string title, bool value)
{
    componentsStack.top()->printFormattedText("\n%18s =   %s", title.c_str(), value ? "TRUE" : "FALSE");
}
    
void TextStrategy::pr_all_lambda(const std::string title, const double vec[], const int n_lambda)
{
    TextComponent* comp = componentsStack.top();
    comp->printFormattedText("\n%18s = ", title.c_str());
    for (int j = 0; j < n_lambda; j++)
    {
        comp->printFormattedText("  %10g", vec[j]);
    }
}

void TextStrategy::pr_init_lambda_weights(const std::string title, const real vec[], const int n_lambda)
{
    TextComponent* comp = componentsStack.top()->addEmptySection();
    comp->addFormattedTextLeaf("%s (%d):", title.c_str(), n_lambda);
    for (int i = 0; i < n_lambda; i++)
    {
        comp->addFormattedTextLeaf("%s[%d]=%12.5e", title.c_str(), i, vec[i]);
    }
}