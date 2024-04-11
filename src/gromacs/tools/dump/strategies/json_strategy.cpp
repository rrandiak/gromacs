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
    JsonDumpComponent* comp = componentsStack.top()->addJsonObject(gmx::formatString("%s (%d)", title.c_str(), i));
    componentsStack.push(comp);
}

void JsonStrategy::pr_title_n(const std::string title, int n)
{
    JsonDumpComponent* comp = componentsStack.top()->addJsonArray(gmx::formatString("%s (%d)", title.c_str(), n));
    componentsStack.push(comp);
}

void JsonStrategy::pr_title_nxm(const std::string title, int n, int m)
{
    JsonDumpComponent* comp = componentsStack.top()->addJsonArray(gmx::formatString("%s (%dx%d)", title.c_str(), n, m));
    componentsStack.push(comp);
}

void JsonStrategy::close_section()
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
    pr_title(title);
    JsonDumpComponent* comp = componentsStack.top();
    comp->printKeyValue("index", i);

    for (int j = 0; j < n; j++)
    {
        comp->printKeyValue(names[j], values[j]);
    }
    componentsStack.pop();
}

void JsonStrategy::pr_residue(const t_resinfo* resinfo, int n)
{
}

void JsonStrategy::pr_ivec(const std::string title, const int vec[], int n)
{
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
}

void JsonStrategy::pr_rvec_row(const std::string title, const real vec[], int n)
{
}

void JsonStrategy::pr_dvec_row(const std::string title, const double vec[], int n)
{
}

void JsonStrategy::pr_svec_row(const std::string title, const char* vec[], int n)
{
}
    
void JsonStrategy::pr_sa_vec_row(const std::string title, const SimulatedAnnealing sa[], int n)
{
}

void JsonStrategy::pr_ap_vec_row(const std::string title, const float vec[], int n)
{
}
    
void JsonStrategy::pr_posrec_vec_row(const std::string title, const real vec[])
{
}

void JsonStrategy::pr_ivec_block(const std::string title, const int vec[], int n)
{
}

void JsonStrategy::pr_matrix(const std::string title, const rvec* m) {
    // TODO: bMDPformat
    if (false) {
        pr_title(title);
        pr_named_value("x_x", m[XX][XX]);
        pr_named_value("y_y", m[YY][YY]);
        pr_named_value("z_z", m[ZZ][ZZ]);
        pr_named_value("x_y", m[XX][YY]);
        pr_named_value("x_z", m[XX][ZZ]);
        pr_named_value("y_z", m[YY][ZZ]);
        componentsStack.pop();
    } else {
        pr_rvecs(title, m, DIM);
    }
}

void JsonStrategy::pr_kvtree(const gmx::KeyValueTreeObject kvTree)
{
}

void JsonStrategy::pr_moltype(const int moltype, const std::string moltypeName) 
{
}
    
void JsonStrategy::pr_atom(const t_atom* atom, const int i)
{
}

void JsonStrategy::pr_grps(gmx::ArrayRef<const AtomGroupIndices> grps, const char* const* const* grpname)
{
}

void JsonStrategy::pr_grp_opt_agg(
    const rvec acceleration[], const int ngacc,
    const ivec nFreeze[], const int ngfrz,
    const int egp_flags[], const int ngener
)
{
}

void JsonStrategy::pr_groups(const SimulationGroups& groups)
{
}

void JsonStrategy::pr_group_stats(gmx::EnumerationArray<SimulationAtomGroupType, std::vector<int>>* gcount)
{
}

void JsonStrategy::pr_list_i(const std::string title, const int index, gmx::ArrayRef<const int> list)
{
}
    
void JsonStrategy::pr_iparams(t_functype ftype, const t_iparams& iparams)
{
}

void JsonStrategy::pr_functypes(const std::vector<int>& functype, const int n, const std::vector<t_iparams>& iparams)
{
}
    
void JsonStrategy::pr_interaction_list(const std::string& title, const t_functype* functypes, const InteractionList& ilist, const t_iparams* iparams)
{
}

void JsonStrategy::pr_cmap(const gmx_cmap_t* cmap_grid)
{
}
