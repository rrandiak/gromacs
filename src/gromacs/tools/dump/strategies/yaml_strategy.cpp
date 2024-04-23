#include "yaml_strategy.h"
    
YamlStrategy::YamlStrategy(FILE* fp)
{
    YamlComponent* root = new YamlRootComponent(fp);
    componentsStack.push(root);
}

YamlStrategy::~YamlStrategy()
{
    while (componentsStack.size() > 1)
    {
        componentsStack.pop();
    }
    if (!componentsStack.empty())
    {
        YamlComponent* comp = componentsStack.top();
        componentsStack.pop();
        delete comp;
    }
}

bool YamlStrategy::available(const void* p, const std::string title)
{
    if (!p)
    {
        componentsStack.top()->printKeyValue(title, "Not available");
    }
    return (p != nullptr);
}

void YamlStrategy::pr_filename(const std::string filename)
{
    componentsStack.top()->printKeyValue("file", filename);
}

void YamlStrategy::pr_title(const std::string title)
{
    YamlComponent* comp = componentsStack.top()->addYamlObject(title);
    componentsStack.push(comp);
}

void YamlStrategy::pr_title_i(const std::string title, int i)
{
    YamlComponent* comp = componentsStack.top()->addYamlObject(title, i);
    // comp->printKeyValue("index", i);
    componentsStack.push(comp);
}

void YamlStrategy::pr_title_n(const std::string title, int n)
{
    YamlComponent* comp = componentsStack.top()->addYamlArray(gmx::formatString("%ss", title.c_str()));
    componentsStack.push(comp);
}

void YamlStrategy::pr_title_nxm(const std::string title, int n, int m)
{
    YamlComponent* comp = componentsStack.top()->addYamlArray(gmx::formatString("%s (%dx%d)", title.c_str(), n, m));
    componentsStack.push(comp);
}

void YamlStrategy::pr_title_list(const std::string title)
{
    YamlComponent* comp = componentsStack.top()->addYamlArray(title);
    componentsStack.push(comp);
}

void YamlStrategy::pr_title_atom_names(int n)
{
    pr_title_n("atom-name", n);
}

void YamlStrategy::close_section()
{
    componentsStack.pop();
}

void YamlStrategy::close_list()
{
    componentsStack.pop();
}

void YamlStrategy::pr_named_value(const std::string name, const Value& value)
{
    componentsStack.top()->printKeyValue(name, value);
}

void YamlStrategy::pr_named_value_short_format(const std::string name, const Value& value)
{
    pr_named_value(name, value);
}

void YamlStrategy::pr_named_value_scientific(const std::string name, const real& value)
{
    pr_named_value(name, value);
}
    
void YamlStrategy::pr_count(const std::string name, const Value& value)
{
    pr_named_value(gmx::formatString("%s-count", name.c_str()), value);
}
    
void YamlStrategy::pr_attribute(const std::string name, const Value& value)
{
    pr_named_value(name, value);
}

void YamlStrategy::pr_attribute_quoted(const std::string name, const std::string& value)
{
    pr_named_value(name, "\"" + value + "\"");
}

void YamlStrategy::pr_vec_attributes(const std::string title, int i, const char** names, char** values, int n)
{
    pr_title(title);
    YamlComponent* comp = componentsStack.top();
    comp->printKeyValue("index", i);

    for (int j = 0; j < n; j++)
    {
        comp->printKeyValue(names[j], values[j]);
    }
    componentsStack.pop();
}

void YamlStrategy::pr_residue(const t_resinfo* resinfo, int i)
{
    YamlInlineObject* inlineObject = componentsStack.top()->addYamlInlineObject();
    inlineObject->printKeyValue("name", *(resinfo->name));
    inlineObject->printKeyValue("nr", resinfo->nr);
    inlineObject->printKeyValue("ic", gmx::formatString("'%c'", resinfo->ic == '\0' ? ' ' : resinfo->ic));
    delete inlineObject;
}

void YamlStrategy::pr_ivec(const std::string title, const int vec[], int n)
{
    if (available(vec, title))
    {
        pr_title_n(title, n);
        YamlInlineArrayComponent* comp = componentsStack.top()->addYamlInlineArray();
        for (int i = 0; i < n; i++)
        {
            comp->printValue(vec[i]);
        }
        delete comp;
        componentsStack.pop();
    }
}

void YamlStrategy::pr_rvec(const std::string title, const real vec[], int n)
{
    if (available(vec, title))
    {
        pr_title_n(title, n);
        YamlInlineArrayComponent* comp = componentsStack.top()->addYamlInlineArray();
        for (int i = 0; i < n; i++)
        {
            comp->printValue(vec[i]);
        }
        delete comp;
        componentsStack.pop();
    }
}
    
void YamlStrategy::pr_ivecs(const std::string title, const ivec vec[], int n)
{
    if (available(vec, title))
    {
        int i, j;
        pr_title_nxm(title, n, DIM);

        for (i = 0; i < n; i++)
        {
            YamlInlineArrayComponent* comp = componentsStack.top()->addYamlInlineArray();
            for (j = 0; j < DIM; j++)
            {
                comp->printValue(vec[i][j]);
            }
            delete comp;
        }

        componentsStack.pop();
    }
}

void YamlStrategy::pr_rvecs(const std::string title, const rvec vec[], int n)
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
            YamlInlineArrayComponent* comp = componentsStack.top()->addYamlInlineArray();
            for (j = 0; j < DIM; j++)
            {
                comp->printValue(vec[i][j]);
            }
            delete comp;
        }

        componentsStack.pop();
    }
}

void YamlStrategy::pr_ivec_row(const std::string title, const int vec[], int n)
{
    YamlInlineArrayComponent* comp = componentsStack.top()->addYamlInlineArray(title);
    for (int i = 0; i < n; i++)
    {
        comp->printValue(vec[i]);
    }
    delete comp;
}

void YamlStrategy::pr_rvec_row(const std::string title, const real vec[], int n)
{
    if (available(vec, title))
    {
        YamlInlineArrayComponent* comp = componentsStack.top()->addYamlInlineArray(title);
        for (int i = 0; i < n; i++)
        {
            comp->printValue(vec[i]);
        }
        delete comp;
    }
}

void YamlStrategy::pr_dvec_row(const std::string title, const double vec[], int n)
{
    if (available(vec, title))
    {
        YamlInlineArrayComponent* comp = componentsStack.top()->addYamlInlineArray(title);
        for (int i = 0; i < n; i++)
        {
            comp->printValue(vec[i]);
        }
        delete comp;
    }
}

void YamlStrategy::pr_svec_row(const std::string title, const char* vec[], int n)
{
    if (available(vec, title))
    {
        YamlInlineArrayComponent* comp = componentsStack.top()->addYamlInlineArray(title);
        for (int i = 0; i < n; i++)
        {
            comp->printValue(vec[i]);
        }
        delete comp;
    }
}

void YamlStrategy::pr_sa_vec_row(const std::string title, const SimulatedAnnealing sa[], int n)
{
    YamlInlineArrayComponent* comp = componentsStack.top()->addYamlInlineArray(title);
    for (int i = 0; i < n; i++)
    {
        comp->printValue(enumValueToString(sa[i]));
    }
    delete comp;
}

void YamlStrategy::pr_ap_vec_row(const std::string title, const float vec[], int n)
{
    YamlInlineArrayComponent* comp = componentsStack.top()->addYamlInlineArray(title);
    for (int i = 0; i < n; i++)
    {
        comp->printValue(vec[i]);
    }
    delete comp;
}
    
void YamlStrategy::pr_posrec_vec_row(const std::string title, const real vec[])
{
    YamlInlineArrayComponent* comp = componentsStack.top()->addYamlInlineArray(title);
    comp->printValue(vec[XX]);
    comp->printValue(vec[YY]);
    comp->printValue(vec[ZZ]);
    delete comp;
}

void YamlStrategy::pr_ivec_block(const std::string title, const int vec[], int n)
{
}

void YamlStrategy::pr_matrix(const std::string title, const rvec* m)
{
    if (bMDPformat)
    {
        YamlArrayComponent* comp = componentsStack.top()->addYamlArray(title);
        YamlInlineArrayComponent* array1 = comp->addYamlInlineArray();
        array1->printValue(m[XX][XX]);
        array1->printValue(m[YY][YY]);
        array1->printValue(m[ZZ][ZZ]);
        YamlInlineArrayComponent* array2 = comp->addYamlInlineArray();
        array2->printValue(m[XX][YY]);
        array2->printValue(m[XX][ZZ]);
        array2->printValue(m[YY][ZZ]);
        delete array2;
        delete array1;
        delete comp;
    }
    else
    {
        pr_rvecs(title, m, DIM);
    }
}

void YamlStrategy::pr_kvtree(const gmx::KeyValueTreeObject kvTree)
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
                YamlInlineArrayComponent *inlineArray =  componentsStack.top()->addYamlInlineArray(prop.key().c_str());
                for (const auto& elem : value.asArray().values())
                {
                    GMX_RELEASE_ASSERT(
                            !elem.isObject() && !elem.isArray(),
                            "Only arrays of simple types and array of objects are implemented. "
                            "Arrays of arrays and mixed arrays are not supported.");
                    inlineArray->printValue(simpleValueToString(elem).c_str());
                }
            }
            else
            {
                std::string strValue = simpleValueToString(value);
                if (value.isType<std::string>() && strValue.empty())
                {
                    pr_named_value(prop.key(), "\"\"");
                }
                else
                {
                    pr_named_value(prop.key(), strValue);
                }
            }

        }
    }
}

void YamlStrategy::pr_moltype(const int moltype, const std::string moltypeName)
{
    componentsStack.top()->printKeyValue("moltypeName", moltypeName.c_str());
}

void YamlStrategy::pr_atom(const t_atom* atom, const int i)
{
    // pr_title_i("atom", i);
    
    YamlInlineObject* comp = componentsStack.top()->addYamlInlineObject();
    comp->printKeyValue("index", i);
    comp->printKeyValue("type", atom->type);
    comp->printKeyValue("typeB", atom->typeB);
    comp->printKeyValue("ptype", enumValueToString(atom->ptype));
    comp->printKeyValue("m", atom->m);
    comp->printKeyValue("q", atom->q);
    comp->printKeyValue("mB", atom->mB);
    comp->printKeyValue("qB", atom->qB);
    comp->printKeyValue("resind", atom->resind);
    comp->printKeyValue("atomnumber", atom->atomnumber);
    delete comp;

    // componentsStack.pop();
}

void YamlStrategy::pr_grps(gmx::ArrayRef<const AtomGroupIndices> grps, const char* const* const* grpname)
{
    pr_title_list("grp-props");
    int index = 0;
    for (const auto& group : grps)
    {
        YamlComponent* comp = componentsStack.top()->addYamlObject("grp");
        comp->printKeyValue("type", shortName(static_cast<SimulationAtomGroupType>(index)));
        comp->printKeyValue("nr", group.size());
        YamlInlineArrayComponent* arrayComp = comp->addYamlInlineArray("name");
        for (const auto& entry : group)
        {
            arrayComp->printValue(*(grpname[entry]));
        }
        delete arrayComp;
        delete comp;
        index++;
    }
    close_list();
}

void YamlStrategy::pr_grp_opt_agg(
    const rvec acceleration[], const int ngacc,
    const ivec nFreeze[], const int ngfrz,
    const int egp_flags[], const int ngener
)
{
    int i, m;

    YamlInlineArrayComponent* comp = componentsStack.top()->addYamlInlineArray("acc");
    for (i = 0; i < ngacc; i++)
    {
        for (m = 0; m < DIM; m++)
        {
            comp->printValue(acceleration[i][m]);
        }
    }
    delete comp;

    comp = componentsStack.top()->addYamlInlineArray("nfreeze");
    for (i = 0; i < ngfrz; i++)
    {
        for (m = 0; m < DIM; m++)
        {
            comp->printValue(nFreeze[i][m] ? "Y" : "N");
        }
    }
    delete comp;

    YamlArrayComponent* array = componentsStack.top()->addYamlArray("energygrp-flags");
    for (i = 0; (i < ngener); i++)
    {
        YamlInlineArrayComponent* inlineArray = array->addYamlInlineArray();
        for (m = 0; m < ngener; m++)
        {
            inlineArray->printValue(egp_flags[ngener * i + m]);
        }
        delete inlineArray;
    }
    delete array;
}

void YamlStrategy::pr_groups(const SimulationGroups& groups)
{
    pr_title("groups");

    YamlInlineObject* allocated = componentsStack.top()->addYamlInlineObject("allocated");
    int nat_max = 0;
    for (auto group : keysOf(groups.groups))
    {
        allocated->printKeyValue(shortName(group), groups.numberOfGroupNumbers(group));
        nat_max = std::max(nat_max, groups.numberOfGroupNumbers(group));
    }
    delete allocated;

    pr_title_list("groups");

    YamlInlineObject* groupnr;
    if (nat_max == 0)
    {
        groupnr = componentsStack.top()->addYamlInlineObject("groupnr");
        groupnr->printKeyValue("index", "all");
        for (auto gmx_unused group : keysOf(groups.groups))
        {
            groupnr->printKeyValue(shortName(group), 0);
        }
        delete groupnr;
    }
    else
    {
        for (int i = 0; i < nat_max; i++)
        {
            groupnr = componentsStack.top()->addYamlInlineObject("groupnr");
            groupnr->printKeyValue("index", i);
            for (auto group : keysOf(groups.groups))
            {
                groupnr->printKeyValue(shortName(group),
                    !groups.groupNumbers[group].empty() ? groups.groupNumbers[group][i] : 0);
            }
            delete groupnr;
        }
    }

    close_list();
    componentsStack.pop();
}

void YamlStrategy::pr_group_stats(gmx::EnumerationArray<SimulationAtomGroupType, std::vector<int>>* gcount)
{
    pr_title("Group statistics");

    int atot;
    for (auto group : keysOf(*gcount))
    {
        atot = 0;
        pr_title(shortName(group));

        YamlInlineArrayComponent* inlineArray = componentsStack.top()->addYamlInlineArray("atoms");
        for (const auto& entry : (*gcount)[group])
        {
            inlineArray->printValue(entry);
            atot += entry;
        }
        delete inlineArray;

        componentsStack.top()->printKeyValue("total atoms", atot);
        close_section();
    }

    close_section();
}

void YamlStrategy::pr_list_i(const std::string title, const int index, gmx::ArrayRef<const int> list)
{
    pr_title(title);

    componentsStack.top()->printKeyValue("index", index);
    componentsStack.top()->printKeyValue("num", list.size());

    YamlInlineArrayComponent* inlineArray = componentsStack.top()->addYamlInlineArray("values");
    for (const int item : list)
    {
        inlineArray->printValue(item);
    }
    delete inlineArray;

    close_section();
}
    
void YamlStrategy::pr_iparams(t_functype ftype, const t_iparams& iparams)
{
    YamlComponent* comp = componentsStack.top();
    std::vector<KeyFormatValue> kfvs = getInteractionParameters(ftype, iparams);

    for (size_t j = 0; j < kfvs.size(); j++)
    {
        if (std::holds_alternative<std::array<real, 3>>(kfvs[j].value))
        {
            YamlInlineArrayComponent* inlineArray = comp->addYamlInlineArray(kfvs[j].key);
            std::array<real, 3> arr = std::get<std::array<real, 3>>(kfvs[j].value);
            for (int k = 0; k < DIM; k++)
            {
                inlineArray->printValue(arr[k]);
            }
            delete inlineArray;
        }
        else
        {
            comp->printKeyValue(kfvs[j].key, kfvs[j].value);
        }
    }
}

void YamlStrategy::pr_functypes(const std::vector<int>& functype, const int n, const std::vector<t_iparams>& iparams)
{
    YamlArrayComponent* array = componentsStack.top()->addYamlArray("functypes");

    for (int i = 0; i < n; i++)
    {
        YamlObjectComponent* object;
        if (bShowNumbers)
        {
            object = array->addYamlObject("functype", i);
        }
        else
        {
            object = array->addYamlObject("functype");
        }
        componentsStack.push(object);

        pr_iparams(functype[i], iparams[i]);

        close_section();
    }

    // delete array;
}

void YamlStrategy::pr_interaction_list(const std::string& title, const t_functype* functypes, const InteractionList& ilist, const t_iparams* iparams)
{
    pr_title(title.c_str());
    componentsStack.top()->printKeyValue("nr", ilist.size());

    if (ilist.empty())
    {
        close_section();
        return;
    }

    YamlArrayComponent* array = componentsStack.top()->addYamlArray("iatoms");

    int j = 0;
    for (int i = 0; i < ilist.size();)
    {
        YamlInlineObject* inlineObject = array->addYamlInlineObject();
        const int type  = ilist.iatoms[i];
        const int ftype = functypes[type];
        if (bShowNumbers)
        {
            inlineObject->printKeyValue("index", j);
            inlineObject->printKeyValue("type", type);
        }
        j++;
        inlineObject->printKeyValue("function", interaction_function[ftype].name);
        YamlInlineArrayComponent* inlineArray = inlineObject->addYamlInlineArray("atoms");
        for (int k = 0; k < interaction_function[ftype].nratoms; k++)
        {
            inlineArray->printValue(ilist.iatoms[i + 1 + k]);
        }
        delete inlineArray;
        if (bShowParameters)
        {
            pr_iparams(ftype, iparams[type]);
        }
        delete inlineObject;
        i += 1 + interaction_function[ftype].nratoms;
    }

    delete array;
    close_section();
}

void YamlStrategy::pr_cmap(const gmx_cmap_t* cmap_grid)
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
            YamlInlineObject* inlineObject = componentsStack.top()->addYamlInlineObject();

            if ((j % cmap_grid->grid_spacing) == 0)
            {
                inlineObject->printKeyValue("idx", idx);
                idx += dx;
            }

            inlineObject->printKeyValue("V", cmap_grid->cmapdata[i].cmap[j * 4]);
            inlineObject->printKeyValue("dVdx", cmap_grid->cmapdata[i].cmap[j * 4 + 1]);
            inlineObject->printKeyValue("dVdy", cmap_grid->cmapdata[i].cmap[j * 4 + 2]);
            inlineObject->printKeyValue("d2dV", cmap_grid->cmapdata[i].cmap[j * 4 + 3]);

            delete inlineObject;
        }
        close_section();
    }

    close_section();
}
