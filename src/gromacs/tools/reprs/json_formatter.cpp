#include "json_formatter.h"

#include "gromacs/tools/reprs/components/json_components.h"
#include "gromacs/tools/reprs/repr_formatter.h"
#include "gromacs/tools/reprs/reprs/iparams.h"
#include "gromacs/utility/basedefinitions.h"
#include "gromacs/utility/stringutil.h"

JsonFormatter::JsonFormatter(FILE* file_pointer)
{
    JsonComponent* root = new JsonRootComponent(file_pointer);
    componentsStack.push(root);
}

JsonFormatter::~JsonFormatter()
{
    GMX_RELEASE_ASSERT(componentsStack.empty(),
                       "Components stack of strategies should be empty at the end. "
                       "Some dump section is not being closed properly.");
}

bool JsonFormatter::available(const void* pointer, const std::string title)
{
    if (!pointer)
    {
        componentsStack.top()->printKeyValue(title, "Not available");
    }
    return (pointer != nullptr);
}

void JsonFormatter::pr_filename(const std::string filename)
{
    componentsStack.top()->printKeyValue("filename", filename);
}

void JsonFormatter::pr_title(const std::string title)
{
    JsonComponent* comp = componentsStack.top()->addJsonObject(title);
    componentsStack.push(comp);
}

void JsonFormatter::pr_title_i([[maybe_unused]] const std::string title, int index)
{
    JsonComponent* comp = componentsStack.top()->addJsonObject();
    comp->printKeyValue("index", index);
    componentsStack.push(comp);
}

void JsonFormatter::pr_title_n(const std::string title, [[maybe_unused]] const int n)
{
    JsonComponent* comp = componentsStack.top()->addJsonArray(gmx::formatString("%ss", title.c_str()));
    componentsStack.push(comp);
}

void JsonFormatter::pr_title_nxm(const std::string title, int n, int m)
{
    JsonComponent* comp =
            componentsStack.top()->addJsonArray(gmx::formatString("%s (%dx%d)", title.c_str(), n, m));
    componentsStack.push(comp);
}

void JsonFormatter::pr_title_list(const std::string title)
{
    JsonArrayComponent* comp = componentsStack.top()->addJsonArray(title);
    componentsStack.push(comp);
}

void JsonFormatter::pr_title_atom_names(int n)
{
    pr_title_n("atom-name", n);
}

void JsonFormatter::pr_title_pull_group(std::string title, int i)
{
    pr_title_i(title, i);
}

void JsonFormatter::pr_title_pull_coord(std::string title, int i)
{
    pr_title_i(title, i);
}

void JsonFormatter::pr_title_rot_group(std::string title, int i)
{
    pr_title_i(title, i);
}

void JsonFormatter::pr_title_awh(std::string title)
{
    pr_title(title);
}

void JsonFormatter::pr_title_all_lambdas(std::string title)
{
    pr_title(title);
}

void JsonFormatter::close_section()
{
    JsonComponent* comp = componentsStack.top();
    componentsStack.pop();
    if (componentsStack.empty())
    {
        delete comp;
    }
}

void JsonFormatter::close_list()
{
    close_section();
}

void JsonFormatter::pr_key_value(const std::string name, const Value& value)
{
    componentsStack.top()->printKeyValue(name, value);
}

void JsonFormatter::pr_key_value_short_format(const std::string name, const Value& value)
{
    pr_key_value(name, value);
}

void JsonFormatter::pr_key_value_scientific(const std::string name, const real& value)
{
    pr_key_value(name, gmx::formatString("%e", value));
}

void JsonFormatter::pr_count(const std::string name, const Value& value)
{
    pr_key_value(gmx::formatString("%s.count", name.c_str()), value);
}

void JsonFormatter::pr_attribute(const std::string name, const Value& value)
{
    pr_key_value(name, value);
}

void JsonFormatter::pr_attribute_quoted(const std::string name, const std::string& value)
{
    pr_key_value(name, value);
}

void JsonFormatter::pr_vec_attributes([[maybe_unused]] const std::string title,
                                      int                                index,
                                      const char**                       names,
                                      char**                             values,
                                      int                                n)
{
    JsonComponent* comp = componentsStack.top()->addJsonObject();
    comp->printKeyValue("index", index);

    for (int j = 0; j < n; j++)
    {
        comp->printKeyValue(names[j], values[j]);
    }
}

void JsonFormatter::pr_residue(const t_resinfo* resinfo, [[maybe_unused]] const int index)
{
    JsonComponent* comp = componentsStack.top()->addJsonObject();
    comp->printKeyValue("name", *(resinfo->name));
    comp->printKeyValue("nr", resinfo->nr);
    comp->printKeyValue("ic", gmx::formatString("'%c'", resinfo->ic == '\0' ? ' ' : resinfo->ic));
}

void JsonFormatter::pr_ivec(const std::string title, const int vec[], int n)
{
    if (available(vec, title))
    {
        pr_title_n(title, n);
        for (int i = 0; i < n; i++)
        {
            componentsStack.top()->printValue(vec[i]);
        }
        componentsStack.pop();
    }
}

void JsonFormatter::pr_rvec(const std::string title, const real vec[], int n)
{
    if (available(vec, title))
    {
        pr_title_n(title, n);
        for (int i = 0; i < n; i++)
        {
            componentsStack.top()->printValue(vec[i]);
        }
        componentsStack.pop();
    }
}

void JsonFormatter::pr_ivecs(const std::string title, const ivec vec[], int n)
{
    if (available(vec, title))
    {
        int i, j;
        pr_title_nxm(title, n, DIM);

        for (i = 0; i < n; i++)
        {
            JsonArrayComponent* array = componentsStack.top()->addJsonArray();
            for (j = 0; j < DIM; j++)
            {
                array->printValue(vec[i][j]);
            }
        }

        componentsStack.pop();
    }
}

void JsonFormatter::pr_rvecs(const std::string title, const rvec vec[], int n)
{
    const std::string fshort = "%12.5e";
    const std::string flong  = "%15.8e";
    const std::string format = (getenv("GMX_PRINT_LONGFORMAT") != nullptr) ? flong : fshort;
    int               i, j;

    if (available(vec, title))
    {
        pr_title_nxm(title, n, DIM);

        for (i = 0; i < n; i++)
        {
            JsonArrayComponent* array = componentsStack.top()->addJsonArray();
            for (j = 0; j < DIM; j++)
            {
                array->printValue(vec[i][j]);
            }
        }

        componentsStack.pop();
    }
}

void JsonFormatter::pr_ivec_row(const std::string title, const int vec[], int n)
{
    JsonArrayComponent* array = componentsStack.top()->addJsonArray(title);
    for (int i = 0; i < n; i++)
    {
        array->printValue(vec[i]);
    }
}

void JsonFormatter::pr_rvec_row(const std::string title, const real vec[], int n)
{
    if (available(vec, title))
    {
        JsonArrayComponent* array = componentsStack.top()->addJsonArray(title);
        for (int i = 0; i < n; i++)
        {
            array->printValue(vec[i]);
        }
    }
}

void JsonFormatter::pr_dvec_row(const std::string title, const double vec[], int n)
{
    if (available(vec, title))
    {
        JsonArrayComponent* array = componentsStack.top()->addJsonArray(title);
        for (int i = 0; i < n; i++)
        {
            array->printValue(vec[i]);
        }
    }
}

void JsonFormatter::pr_svec_row(const std::string title, const char* vec[], int n)
{
    if (available(vec, title))
    {
        JsonArrayComponent* array = componentsStack.top()->addJsonArray(title);
        for (int i = 0; i < n; i++)
        {
            array->printValue(vec[i]);
        }
    }
}

void JsonFormatter::pr_sa_vec_row(const std::string title, const SimulatedAnnealing sa[], int n)
{
    JsonArrayComponent* array = componentsStack.top()->addJsonArray(title);
    for (int i = 0; i < n; i++)
    {
        array->printValue(enumValueToString(sa[i]));
    }
}

void JsonFormatter::pr_ap_vec_row(const std::string          title,
                                  const float                vec[],
                                  const int                  n,
                                  [[maybe_unused]] const int index)
{
    JsonArrayComponent* array = componentsStack.top()->addJsonArray(title);
    for (int i = 0; i < n; i++)
    {
        array->printValue(vec[i]);
    }
}

void JsonFormatter::pr_posrec_vec_row(const std::string title, const real vec[])
{
    JsonArrayComponent* array = componentsStack.top()->addJsonArray(title);
    array->printValue(vec[XX]);
    array->printValue(vec[YY]);
    array->printValue(vec[ZZ]);
}


void JsonFormatter::pr_block([[maybe_unused]] std::string title, [[maybe_unused]] const t_block* block)
{
}

void JsonFormatter::pr_ivec_block(const std::string         title,
                                  const int                 vec[],
                                  const int                 n,
                                  [[maybe_unused]] gmx_bool bShowNumbers)
{
    pr_ivec(title, vec, n);
}

void JsonFormatter::pr_matrix(const std::string title, const rvec* m)
{
    if (bMDPformat)
    {
        pr_title(title);
        pr_key_value("x_x", m[XX][XX]);
        pr_key_value("y_y", m[YY][YY]);
        pr_key_value("z_z", m[ZZ][ZZ]);
        pr_key_value("x_y", m[XX][YY]);
        pr_key_value("x_z", m[XX][ZZ]);
        pr_key_value("y_z", m[YY][ZZ]);
        close_section();
    }
    else
    {
        pr_rvecs(title, m, DIM);
    }
}

void JsonFormatter::pr_kvtree(const gmx::KeyValueTreeObject kvTree)
{
    for (const auto& prop : kvTree.properties())
    {
        const auto& value = prop.value();
        if (value.isObject())
        {
            pr_title(prop.key());
            pr_kvtree(value.asObject());
            close_section();
        }
        else if (value.isArray()
                 && std::all_of(value.asArray().values().begin(),
                                value.asArray().values().end(),
                                [](const auto& elem) { return elem.isObject(); }))
        {
            pr_title(prop.key());
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
                JsonArrayComponent* array = componentsStack.top()->addJsonArray(prop.key());
                for (const auto& elem : value.asArray().values())
                {
                    GMX_RELEASE_ASSERT(
                            !elem.isObject() && !elem.isArray(),
                            "Only arrays of simple types and array of objects are implemented. "
                            "Arrays of arrays and mixed arrays are not supported.");
                    array->printValue(simpleValueToString(elem).c_str());
                }
            }
            else
            {
                std::string strValue = simpleValueToString(value);
                if (value.isType<std::string>() && strValue.empty())
                {
                    pr_key_value(prop.key(), "");
                }
                else
                {
                    pr_key_value(prop.key(), strValue);
                }
            }
        }
    }
}

void JsonFormatter::pr_moltype(const int moltype, const std::string moltypeName)
{
    componentsStack.top()->printKeyValue("moltypeIndex", moltype);
    componentsStack.top()->printKeyValue("moltypeName", moltypeName.c_str());
}

void JsonFormatter::pr_atom(const t_atom* atom, [[maybe_unused]] const int index)
{
    JsonObjectComponent* comp = componentsStack.top()->addJsonObject();
    comp->printKeyValue("type", atom->type);
    comp->printKeyValue("typeB", atom->typeB);
    comp->printKeyValue("ptype", enumValueToString(atom->ptype));
    comp->printKeyValue("m", atom->m);
    comp->printKeyValue("q", atom->q);
    comp->printKeyValue("mB", atom->mB);
    comp->printKeyValue("qB", atom->qB);
    comp->printKeyValue("resind", atom->resind);
    comp->printKeyValue("atomnumber", atom->atomnumber);
}

void JsonFormatter::pr_grps(gmx::ArrayRef<const AtomGroupIndices> grps, const char* const* const* grpname)
{
    int index = 0;
    for (const auto& group : grps)
    {
        JsonObjectComponent* comp = componentsStack.top()->addJsonObject("grp");
        comp->printKeyValue("type", shortName(static_cast<SimulationAtomGroupType>(index)));
        comp->printKeyValue("nr", group.size());
        JsonArrayComponent* arrayComp = comp->addJsonArray("name");
        for (const auto& entry : group)
        {
            arrayComp->printValue(*(grpname[entry]));
        }
        index++;
    }
}

void JsonFormatter::pr_grp_opt_agg(const rvec acceleration[],
                                   const int  ngacc,
                                   const ivec nFreeze[],
                                   const int  ngfrz,
                                   const int  egp_flags[],
                                   const int  ngener)
{
    int i, m;

    JsonArrayComponent* array = componentsStack.top()->addJsonArray("acc");
    for (i = 0; i < ngacc; i++)
    {
        for (m = 0; m < DIM; m++)
        {
            array->printValue(acceleration[i][m]);
        }
    }

    array = componentsStack.top()->addJsonArray("nfreeze");
    for (i = 0; i < ngfrz; i++)
    {
        for (m = 0; m < DIM; m++)
        {
            array->printValue(nFreeze[i][m] ? "Y" : "N");
        }
    }

    array = componentsStack.top()->addJsonArray("energygrp-flags");
    for (i = 0; (i < ngener); i++)
    {
        JsonArrayComponent* innerArray = array->addJsonArray();
        for (m = 0; m < ngener; m++)
        {
            innerArray->printValue(egp_flags[ngener * i + m]);
        }
    }
}

void JsonFormatter::pr_groups(const SimulationGroups& groups)
{
    pr_title("groups");

    JsonObjectComponent* allocated = componentsStack.top()->addJsonObject("allocated");
    int                  nat_max   = 0;
    for (auto group : keysOf(groups.groups))
    {
        allocated->printKeyValue(shortName(group), groups.numberOfGroupNumbers(group));
        nat_max = std::max(nat_max, groups.numberOfGroupNumbers(group));
    }

    JsonObjectComponent* groupnr;
    if (nat_max == 0)
    {
        groupnr = componentsStack.top()->addJsonObject("groupnr");
        groupnr->printKeyValue("index", "all");
        for (auto gmx_unused group : keysOf(groups.groups))
        {
            groupnr->printKeyValue(shortName(group), 0);
        }
    }
    else
    {
        for (int i = 0; i < nat_max; i++)
        {
            groupnr = componentsStack.top()->addJsonObject("groupnr");
            groupnr->printKeyValue("index", i);
            for (auto group : keysOf(groups.groups))
            {
                groupnr->printKeyValue(
                        shortName(group),
                        !groups.groupNumbers[group].empty() ? groups.groupNumbers[group][i] : 0);
            }
        }
    }

    close_section();
}

void JsonFormatter::pr_group_stats(gmx::EnumerationArray<SimulationAtomGroupType, std::vector<int>>* gcount)
{
    pr_title("Group statistics");

    int atot;
    for (auto group : keysOf(*gcount))
    {
        atot = 0;
        pr_title(shortName(group));

        JsonArrayComponent* array = componentsStack.top()->addJsonArray("atoms");
        for (const auto& entry : (*gcount)[group])
        {
            array->printValue(entry);
            atot += entry;
        }

        componentsStack.top()->printKeyValue("total atoms", atot);
        close_section();
    }

    close_section();
}

void JsonFormatter::pr_list_i([[maybe_unused]] const std::string title,
                              [[maybe_unused]] const int         index,
                              gmx::ArrayRef<const int>           list)
{
    JsonArrayComponent* array = componentsStack.top()->addJsonArray();
    for (const int item : list)
    {
        array->printValue(item);
    }
}

void JsonFormatter::pr_iparam(std::string name, [[maybe_unused]] std::string format, IParamValue value)
{
    if (std::holds_alternative<int>(value))
    {
        componentsStack.top()->printKeyValue(name, std::get<int>(value));
    }
    else if (std::holds_alternative<real>(value))
    {
        componentsStack.top()->printKeyValue(name, std::get<real>(value));
    }
}

void JsonFormatter::pr_iparam_reals_of_dim(std::string name, [[maybe_unused]] std::string format, real vec[3])
{
    JsonArrayComponent* array = componentsStack.top()->addJsonArray(name);
    for (int i = 0; i < DIM; i++)
    {
        array->printValue(vec[i]);
    }
}

void JsonFormatter::pr_functypes(const t_functype* functypes, const int n, const t_iparams* iparams)
{
    pr_title_list("functypes");

    for (int i = 0; i < n; i++)
    {
        if (bShowNumbers)
        {
            pr_title_i("functype", i);
        }
        else
        {
            pr_title("functype");
        }
        InteractionParamsRepr(functypes[i], iparams[i]).build(this);
        close_section();
    }

    close_list();
}

void JsonFormatter::pr_interaction_list(const std::string      title,
                                        const t_functype*      functypes,
                                        const InteractionList& ilist,
                                        const t_iparams*       iparams)
{
    pr_title(title);
    componentsStack.top()->printKeyValue("nr", ilist.size());

    if (ilist.empty())
    {
        close_section();
        return;
    }

    JsonArrayComponent* array = componentsStack.top()->addJsonArray("iatoms");

    int j = 0;
    for (int i = 0; i < ilist.size();)
    {
        JsonObjectComponent* object = array->addJsonObject();
        const int            type   = ilist.iatoms[i];
        const int            ftype  = functypes[type];
        if (bShowNumbers)
        {
            object->printKeyValue("index", j);
            object->printKeyValue("type", type);
        }
        j++;
        object->printKeyValue("function", interaction_function[ftype].name);
        JsonArrayComponent* array = object->addJsonArray("atoms");
        for (int k = 0; k < interaction_function[ftype].nratoms; k++)
        {
            array->printValue(ilist.iatoms[i + 1 + k]);
        }
        if (bShowParameters)
        {
            InteractionParamsRepr(ftype, iparams[type]).build(this);
        }
        i += 1 + interaction_function[ftype].nratoms;
    }

    close_section();
}

void JsonFormatter::pr_cmap(const gmx_cmap_t* cmap_grid)
{
    const real dx = cmap_grid->grid_spacing != 0 ? 360.0 / cmap_grid->grid_spacing : 0;

    const int nelem = cmap_grid->grid_spacing * cmap_grid->grid_spacing;

    if (!available(cmap_grid, "cmap"))
    {
        return;
    }

    pr_title_list("cmap-grids");

    for (gmx::Index i = 0; i < gmx::ssize(cmap_grid->cmapdata); i++)
    {
        real idx = -180.0;
        for (int j = 0; j < nelem; j++)
        {
            JsonObjectComponent* object = componentsStack.top()->addJsonObject();

            if ((j % cmap_grid->grid_spacing) == 0)
            {
                object->printKeyValue("idx", idx);
                idx += dx;
            }

            object->printKeyValue("V", cmap_grid->cmapdata[i].cmap[j * 4]);
            object->printKeyValue("dVdx", cmap_grid->cmapdata[i].cmap[j * 4 + 1]);
            object->printKeyValue("dVdy", cmap_grid->cmapdata[i].cmap[j * 4 + 2]);
            object->printKeyValue("d2dV", cmap_grid->cmapdata[i].cmap[j * 4 + 3]);
        }
    }

    close_list();
}

void JsonFormatter::pr_separate_dvdl(const std::string title, bool value)
{
    pr_key_value(title, value);
}

void JsonFormatter::pr_all_lambda(const std::string title, const double vec[], const int n_lambda)
{
    pr_dvec_row(title, vec, n_lambda);
}

void JsonFormatter::pr_init_lambda_weights(const std::string title, const real vec[], const int n_lambda)
{
    pr_rvec(title, vec, n_lambda);
}
