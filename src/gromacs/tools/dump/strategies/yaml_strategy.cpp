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
    std::string n_title;
    n_title += title;
    n_title += " ";
    n_title += std::to_string(i);
    YamlComponent* comp = componentsStack.top()->addYamlObject(n_title);
    componentsStack.push(comp);
}

void YamlStrategy::pr_title_n(const std::string title, int n)
{
    std::string n_title;
    n_title += title;
    n_title += " (";
    n_title += std::to_string(n);
    n_title += ")";
    YamlComponent* comp = componentsStack.top()->addYamlObject(n_title);
    componentsStack.push(comp);
}

void YamlStrategy::pr_title_nxm(const std::string title, int n, int m)
{
    std::string nxn_title;
    nxn_title += title;
    nxn_title += " (";
    nxn_title += std::to_string(n);
    nxn_title += "x";
    nxn_title += std::to_string(m);
    nxn_title += ")";
    YamlComponent* comp = componentsStack.top()->addYamlObject(nxn_title);
    componentsStack.push(comp);
}

void YamlStrategy::close_section()
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
}
    
void YamlStrategy::pr_attribute(const std::string name, const Value& value)
{
}

void YamlStrategy::pr_attribute_quoted(const std::string name, const std::string& value)
{
}

void YamlStrategy::pr_vec_attributes(const std::string title, int i, const char** names, char** values, int n)
{
}

void YamlStrategy::pr_residue(const t_resinfo* resinfo, int i)
{
}

void YamlStrategy::pr_ivec(const std::string title, const int vec[], int n)
{
}

void YamlStrategy::pr_rvec(const std::string title, const real vec[], int n)
{
    if (available(vec, title))
    {
        pr_title_n(title, n);
        YamlComponent* comp = componentsStack.top();
        for (int i = 0; i < n; i++)
        {
            std::string cord_title;
            cord_title += "x";
            cord_title += std::to_string(i);
            comp->printKeyValue(cord_title.c_str(), vec[i]);
            // fprintf(fp, "%s[%d]=%12.5e\n", title, bShowNumbers ? i : -1, vec[i]);
        }
        componentsStack.pop();
    }
}
    
void YamlStrategy::pr_ivecs(const std::string title, const ivec vec[], int n)
{
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
            for (j = 0; j < DIM; j++)
            {
                std::string cord_title;
                cord_title += "x";
                cord_title += std::to_string(i);
                cord_title += "_";
                cord_title += "y";
                cord_title += std::to_string(j);
                pr_named_value(cord_title.c_str(), vec[i][j]);
            }
        }

        componentsStack.pop();
    }
}

void YamlStrategy::pr_ivec_row(const std::string title, const int vec[], int n)
{
}

void YamlStrategy::pr_rvec_row(const std::string title, const real vec[], int n)
{
}

void YamlStrategy::pr_dvec_row(const std::string title, const double vec[], int n)
{
}

void YamlStrategy::pr_svec_row(const std::string title, const char* vec[], int n)
{
}

void YamlStrategy::pr_sa_vec_row(const std::string title, const SimulatedAnnealing sa[], int n)
{
}

void YamlStrategy::pr_ap_vec_row(const std::string title, const float vec[], int n)
{
}
    
void YamlStrategy::pr_posrec_vec_row(const std::string title, const real vec[])
{
}

void YamlStrategy::pr_ivec_block(const std::string title, const int vec[], int n)
{
}

void YamlStrategy::pr_matrix(const std::string title, const rvec* m)
{
    if (bMDPformat)
    {
        pr_title(title);
        pr_named_value("x_x", m[XX][XX]);
        pr_named_value("y_y", m[YY][YY]);
        pr_named_value("z_z", m[ZZ][ZZ]);
        pr_named_value("x_y", m[XX][YY]);
        pr_named_value("x_z", m[XX][ZZ]);
        pr_named_value("y_z", m[YY][ZZ]);
        componentsStack.pop();
    }
    else
    {
        pr_rvecs(title, m, DIM);
    }
}

void YamlStrategy::pr_kvtree(const gmx::KeyValueTreeObject kvTree)
{
}

void YamlStrategy::pr_moltype(const int moltype, const std::string typeName)
{
}

void YamlStrategy::pr_atom(const t_atom* atom, const int i)
{
}

void YamlStrategy::pr_grps(gmx::ArrayRef<const AtomGroupIndices> grps, const char* const* const* grpname)
{
}

void YamlStrategy::pr_grp_opt_agg(
    const rvec acceleration[], const int ngacc,
    const ivec nFreeze[], const int ngfrz,
    const int egp_flags[], const int ngener
)
{
}

void YamlStrategy::pr_groups(const SimulationGroups& groups)
{
}

void YamlStrategy::pr_group_stats(gmx::EnumerationArray<SimulationAtomGroupType, std::vector<int>>* gcount)
{
}

void YamlStrategy::pr_list_i(const std::string title, const int index, gmx::ArrayRef<const int> list)
{
}
    
void YamlStrategy::pr_iparams(t_functype ftype, const t_iparams& iparams)
{
}

void YamlStrategy::pr_functypes(const std::vector<int>& functype, const int n, const std::vector<t_iparams>& iparams)
{
}

void YamlStrategy::pr_interaction_list(const std::string& title, const t_functype* functypes, const InteractionList& ilist, const t_iparams* iparams)
{
}

void YamlStrategy::pr_cmap(const gmx_cmap_t* cmap_grid)
{
}
