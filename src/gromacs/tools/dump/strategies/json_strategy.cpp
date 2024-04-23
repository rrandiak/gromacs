#include "json_strategy.h"
    
JsonStrategy::JsonStrategy(FILE* fp)
{
    JsonDumpComponent* root = new JsonRootComponent(fp);
    componentsStack.push(root);
}

JsonStrategy::~JsonStrategy()
{
    while (componentsStack.size() > 1)
    {
        componentsStack.pop();
    }
    if (!componentsStack.empty())
    {
        JsonDumpComponent* comp = componentsStack.top();
        componentsStack.pop();
        delete comp;
    }
}

bool JsonStrategy::available(const void* p, const std::string title)
{
    if (!p)
    {
        componentsStack.top()->printKeyValue(title, "Not available");
    }
    return (p != nullptr);
}

void JsonStrategy::pr_filename(const std::string filename)
{
    componentsStack.top()->printKeyValue("filename", filename);
}

void JsonStrategy::pr_title(const std::string title)
{
    JsonDumpComponent* comp = componentsStack.top()->addJsonObject(title);
    componentsStack.push(comp);
}

void JsonStrategy::pr_title_i(const std::string title, int i)
{
    JsonDumpComponent* comp = componentsStack.top()->addJsonObject();
    comp->printKeyValue("index", i);
    componentsStack.push(comp);
}

void JsonStrategy::pr_title_n(const std::string title, int n)
{
    JsonDumpComponent* comp = componentsStack.top()->addJsonArray(gmx::formatString("%ss", title.c_str()));
    componentsStack.push(comp);
}

void JsonStrategy::pr_title_nxm(const std::string title, int n, int m)
{
    JsonDumpComponent* comp = componentsStack.top()->addJsonArray(gmx::formatString("%s (%dx%d)", title.c_str(), n, m));
    componentsStack.push(comp);
}

void JsonStrategy::pr_title_list(const std::string title)
{
    JsonArrayComponent* comp = componentsStack.top()->addJsonArray(title);
    componentsStack.push(comp);
}

void JsonStrategy::pr_title_atom_names(int n)
{
    pr_title_n("atom-name", n);
}

void JsonStrategy::close_section()
{
    componentsStack.pop();
}

void JsonStrategy::close_list()
{
    componentsStack.pop();
}

void JsonStrategy::pr_named_value(const std::string name, const Value& value)
{
    componentsStack.top()->printKeyValue(name, value);
}
    
void JsonStrategy::pr_named_value_short_format(const std::string name, const Value& value) 
{
    pr_named_value(name, value);
}
    
void JsonStrategy::pr_named_value_scientific(const std::string name, const real& value)
{
    pr_named_value(name, gmx::formatString("%e", value));
}
    
void JsonStrategy::pr_count(const std::string name, const Value& value)
{
    pr_named_value(gmx::formatString("%s.count", name.c_str()), value);
}
      
void JsonStrategy::pr_attribute(const std::string name, const Value& value)
{
    pr_named_value(name, value);
}

void JsonStrategy::pr_attribute_quoted(const std::string name, const std::string& value)
{
    pr_named_value(name, value);
}

void JsonStrategy::pr_vec_attributes(const std::string title, int i, const char** names, char** values, int n)
{
    // pr_title(title);
    JsonDumpComponent* comp = componentsStack.top()->addJsonObject();
    comp->printKeyValue("index", i);

    for (int j = 0; j < n; j++)
    {
        comp->printKeyValue(names[j], values[j]);
    }
}

void JsonStrategy::pr_residue(const t_resinfo* resinfo, int n)
{
    JsonDumpComponent* comp = componentsStack.top()->addJsonObject();
    comp->printKeyValue("name", *(resinfo->name));
    comp->printKeyValue("nr", resinfo->nr);
    comp->printKeyValue("ic", gmx::formatString("'%c'", resinfo->ic == '\0' ? ' ' : resinfo->ic));
}

void JsonStrategy::pr_ivec(const std::string title, const int vec[], int n)
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

void JsonStrategy::pr_rvec(const std::string title, const real vec[], int n)
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
    
void JsonStrategy::pr_ivecs(const std::string title, const ivec vec[], int n)
{
    if (available(vec, title))
    {
        int i, j;
        pr_title_nxm(title, n, DIM);

        for (i = 0; i < n; i++)
        {
            // TODO: change to normal array and refactor component
            JsonInlineArray* inlineArray = componentsStack.top()->addInlineArray();
            for (j = 0; j < DIM; j++)
            {
                inlineArray->printValue(vec[i][j]);
            }
        }

        componentsStack.pop();
    }
}

void JsonStrategy::pr_rvecs(const std::string title, const rvec vec[], int n)
{
    const std::string fshort = "%12.5e";
    const std::string flong  = "%15.8e";
    const std::string format = (getenv("GMX_PRINT_LONGFORMAT") != nullptr) ? flong : fshort;
    int         i, j;

    if (available(vec, title))
    {
        pr_title_nxm(title, n, DIM);

        for (i = 0; i < n; i++)
        {
            // TODO: change to normal array and refactor component
            JsonInlineArray* inlineArray = componentsStack.top()->addInlineArray();
            for (j = 0; j < DIM; j++)
            {
                inlineArray->printValue(vec[i][j]);
            }
        }

        componentsStack.pop();
    }
}

void JsonStrategy::pr_ivec_row(const std::string title, const int vec[], int n)
{
    JsonArrayComponent* array = componentsStack.top()->addJsonArray(title);
    for (int i = 0; i < n; i++)
    {
        array->printValue(vec[i]);
    }
}

void JsonStrategy::pr_rvec_row(const std::string title, const real vec[], int n)
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

void JsonStrategy::pr_dvec_row(const std::string title, const double vec[], int n)
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

void JsonStrategy::pr_svec_row(const std::string title, const char* vec[], int n)
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
    
void JsonStrategy::pr_sa_vec_row(const std::string title, const SimulatedAnnealing sa[], int n)
{
    JsonArrayComponent* array = componentsStack.top()->addJsonArray(title);
    for (int i = 0; i < n; i++)
    {
        array->printValue(enumValueToString(sa[i]));
    }
}

void JsonStrategy::pr_ap_vec_row(const std::string title, const float vec[], int n)
{
    JsonArrayComponent* array = componentsStack.top()->addJsonArray(title);
    for (int i = 0; i < n; i++)
    {
        array->printValue(vec[i]);
    }
}
    
void JsonStrategy::pr_posrec_vec_row(const std::string title, const real vec[])
{
    JsonArrayComponent* array = componentsStack.top()->addJsonArray(title);
    array->printValue(vec[XX]);
    array->printValue(vec[YY]);
    array->printValue(vec[ZZ]);
}

void JsonStrategy::pr_ivec_block(const std::string title, const int vec[], int n)
{
}

void JsonStrategy::pr_matrix(const std::string title, const rvec* m) {
    if (bMDPformat) {
        pr_title(title);
        pr_named_value("x_x", m[XX][XX]);
        pr_named_value("y_y", m[YY][YY]);
        pr_named_value("z_z", m[ZZ][ZZ]);
        pr_named_value("x_y", m[XX][YY]);
        pr_named_value("x_z", m[XX][ZZ]);
        pr_named_value("y_z", m[YY][ZZ]);
        close_section();
    } else {
        pr_rvecs(title, m, DIM);
    }
}

void JsonStrategy::pr_kvtree(const gmx::KeyValueTreeObject kvTree)
{
    for (const auto& prop : kvTree.properties())
    {
        const auto& value = prop.value();
        if (value.isObject())
        {
            pr_title(prop.key().c_str());
            pr_kvtree(value.asObject());
            close_section();
        }
        else if (value.isArray()
                 && std::all_of(value.asArray().values().begin(),
                                value.asArray().values().end(),
                                [](const auto& elem) { return elem.isObject(); }))
        {
            pr_title(prop.key().c_str());
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
                JsonArrayComponent* array =  componentsStack.top()->addJsonArray(prop.key().c_str());
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
                    pr_named_value(prop.key(), "");
                }
                else
                {
                    pr_named_value(prop.key(), strValue);
                }
            }

        }
    }
}

void JsonStrategy::pr_moltype(const int moltype, const std::string moltypeName) 
{
    componentsStack.top()->printKeyValue("moltypeIndex", moltype);
    componentsStack.top()->printKeyValue("moltypeName", moltypeName.c_str());
}
    
void JsonStrategy::pr_atom(const t_atom* atom, const int i)
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

void JsonStrategy::pr_grps(gmx::ArrayRef<const AtomGroupIndices> grps, const char* const* const* grpname)
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

void JsonStrategy::pr_grp_opt_agg(
    const rvec acceleration[], const int ngacc,
    const ivec nFreeze[], const int ngfrz,
    const int egp_flags[], const int ngener
)
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

void JsonStrategy::pr_groups(const SimulationGroups& groups)
{
    pr_title("groups");

    JsonObjectComponent* allocated = componentsStack.top()->addJsonObject("allocated");
    int nat_max = 0;
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
                groupnr->printKeyValue(shortName(group),
                    !groups.groupNumbers[group].empty() ? groups.groupNumbers[group][i] : 0);
            }
        }
    }

    close_section();
}

void JsonStrategy::pr_group_stats(gmx::EnumerationArray<SimulationAtomGroupType, std::vector<int>>* gcount)
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

void JsonStrategy::pr_list_i(const std::string title, const int index, gmx::ArrayRef<const int> list)
{
    JsonArrayComponent* array = componentsStack.top()->addJsonArray();
    for (const int item : list)
    {
        array->printValue(item);
    }
}
    
void JsonStrategy::pr_iparams(t_functype ftype, const t_iparams& iparams)
{
    JsonDumpComponent* comp = componentsStack.top();
    std::vector<KeyFormatValue> kfvs = getInteractionParameters(ftype, iparams);

    for (size_t j = 0; j < kfvs.size(); j++)
    {
        if (std::holds_alternative<std::array<real, 3>>(kfvs[j].value))
        {
            JsonArrayComponent* array = comp->addJsonArray(kfvs[j].key);
            std::array<real, 3> arr = std::get<std::array<real, 3>>(kfvs[j].value);
            for (int k = 0; k < DIM; k++)
            {
                array->printValue(arr[k]);
            }
        }
        else
        {
            comp->printKeyValue(kfvs[j].key, kfvs[j].value);
        }
    }
}

void JsonStrategy::pr_functypes(const std::vector<int>& functype, const int n, const std::vector<t_iparams>& iparams)
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
        pr_iparams(functype[i], iparams[i]);
        close_section();
    }

    close_list();
}
    
void JsonStrategy::pr_interaction_list(const std::string& title, const t_functype* functypes, const InteractionList& ilist, const t_iparams* iparams)
{
    pr_title(title.c_str());
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
        const int type  = ilist.iatoms[i];
        const int ftype = functypes[type];
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
            pr_iparams(ftype, iparams[type]);
        }
        i += 1 + interaction_function[ftype].nratoms;
    }

    close_section();
}

void JsonStrategy::pr_cmap(const gmx_cmap_t* cmap_grid)
{
    const real dx = cmap_grid->grid_spacing != 0 ? 360.0 / cmap_grid->grid_spacing : 0;

    const int nelem = cmap_grid->grid_spacing * cmap_grid->grid_spacing;

    if (!available(cmap_grid, "cmap"))
    {
        return;
    }

    pr_title("cmap");

    for (gmx::Index i = 0; i < gmx::ssize(cmap_grid->cmapdata); i++)
    {
        real idx = -180.0;

        if (bShowNumbers)
        {
            pr_title("grid");
        }
        else
        {
            pr_title_i("grid", i);
        }

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
        close_section();
    }

    close_section();
}
