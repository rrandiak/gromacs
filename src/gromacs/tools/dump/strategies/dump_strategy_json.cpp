#include "gromacs/tools/dump/strategies/dump_strategy_json.h"

bool DumpJsonStrategy::available(const void* p, const char* title) {
    if (!p) {
        componentsStack.top()->addJsonLeaf(title, "not_available");
    }
    return (p != nullptr);
}

void DumpJsonStrategy::pr_filename(const char* filename) {
    componentsStack.top()->addJsonLeaf("filename", filename);
}

void DumpJsonStrategy::pr_title(const char* title) {
    JsonDumpComponent* comp = componentsStack.top()->addJsonObject(title);
    componentsStack.push(comp);
}

void DumpJsonStrategy::pr_title_i(const char* title, int i) {
    // fprintf(fp, "%s (%d):\n", title, n);
    std::string n_title;
    n_title += title;
    n_title += " ";
    n_title += std::to_string(i);
    JsonDumpComponent* comp = componentsStack.top()->addJsonObject(n_title);
    componentsStack.push(comp);
}

void DumpJsonStrategy::pr_title_n(const char* title, int n) {
    // fprintf(fp, "%s (%d):\n", title, n);
    std::string n_title;
    n_title += title;
    n_title += " (";
    n_title += std::to_string(n);
    n_title += ")";
    JsonDumpComponent* comp = componentsStack.top()->addJsonObject(n_title);
    componentsStack.push(comp);
}

void DumpJsonStrategy::pr_title_nxn(const char* title, int n1, int n2) {
    // fprintf(fp, "%s (%dx%d):\n", title, n1, n2);
    std::string nxn_title;
    nxn_title += title;
    nxn_title += " (";
    nxn_title += std::to_string(n1);
    nxn_title += "x";
    nxn_title += std::to_string(n2);
    nxn_title += ")";
    JsonDumpComponent* comp = componentsStack.top()->addJsonObject(nxn_title);
    componentsStack.push(comp);
}

void DumpJsonStrategy::close_section() {
    // delete componentsStack.top();
    componentsStack.pop();
}

void DumpJsonStrategy::pr_named_value(const char* name, const Value& value) {
    componentsStack.top()->addJsonLeaf(name, value);
}
    
void DumpJsonStrategy::pr_attribute(const char* name, const Value& value)
{
}

void DumpJsonStrategy::pr_attribute_quoted(const char* name, const std::string& value)
{
}

void DumpJsonStrategy::pr_vec_attributes(const char* title, int i, const char** names, char** values, int n)
{
}

void DumpJsonStrategy::pr_ivec(const char* title, const int vec[], int n)
{
}

void DumpJsonStrategy::pr_ivec_row(const char* title, const int vec[], int n)
{
}
    
void DumpJsonStrategy::pr_ivecs(const char* title, const ivec vec[], int n)
{
}

void DumpJsonStrategy::pr_ivec_block(const char* title, const int vec[], int n)
{
}

void DumpJsonStrategy::pr_rvec(const char* title, const real vec[], int n) {
    if (available(vec, title)) {
        pr_title_n(title, n);
        for (int i = 0; i < n; i++) {
            std::string cord_title;
            cord_title += "x";
            cord_title += std::to_string(i);
            pr_named_value(cord_title.c_str(), vec[i]);
            // fprintf(fp, "%s[%d]=%12.5e\n", title, bShowNumbers ? i : -1, vec[i]);
        }
        componentsStack.pop();
    }
}

void DumpJsonStrategy::pr_rvec_row(const char* title, const real vec[], int n)
{
}

void DumpJsonStrategy::pr_rvecs(const char* title, const rvec vec[], int n) {
    const char* fshort = "%12.5e";
    const char* flong  = "%15.8e";
    const char* format = (getenv("GMX_PRINT_LONGFORMAT") != nullptr) ? flong : fshort;
    int         i, j;

    if (available(vec, title)) {
        pr_title_nxn(title, n, DIM);

        for (i = 0; i < n; i++) {
            for (j = 0; j < DIM; j++) {
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

void DumpJsonStrategy::pr_matrix(const char* title, const rvec* m) {
    if (bMDPformat) {
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

void DumpJsonStrategy::pr_kvtree(const gmx::KeyValueTreeObject kvTree)
{
}

void DumpJsonStrategy::pr_tpx_header(const TpxFileHeader* sh)
{
}
    
void DumpJsonStrategy::pr_sim_annealing(const char* title, const SimulatedAnnealing sa[], int n)
{
}

void DumpJsonStrategy::pr_functypes(const std::vector<int>& functype, const int n, const std::vector<t_iparams>& iparams)
{
}

void DumpJsonStrategy::pr_moltype(const int moltype, const char* moltypeName) 
{
}

void DumpJsonStrategy::pr_grps(gmx::ArrayRef<const AtomGroupIndices> grps, const char* const* const* grpname)
{
}

void DumpJsonStrategy::pr_group_stats(gmx::EnumerationArray<SimulationAtomGroupType, std::vector<int>>* gcount)
{
}

void DumpJsonStrategy::pr_anneal_points(const char* title, const float vec[], int n)
{
}

void DumpJsonStrategy::pr_grp_opt_agg(
    const rvec acceleration[], const int ngacc,
    const ivec nFreeze[], const int ngfrz,
    const int egp_flags[], const int ngener
)
{
}

void DumpJsonStrategy::pr_atoms(const t_atoms* atoms)
{}
    
void DumpJsonStrategy::pr_list_i(const char* title, const int index, gmx::ArrayRef<const int> list)
{}
    
void DumpJsonStrategy::pr_interaction_list(const std::string& title, const t_functype* functypes, const InteractionList& ilist, const t_iparams* iparams)
{}

void DumpJsonStrategy::pr_groups(const SimulationGroups& groups)
{}

void DumpJsonStrategy::pr_resinfo(const t_resinfo* resinfo, int n)
{}

void DumpJsonStrategy::pr_cmap(const gmx_cmap_t* cmap_grid)
{}
