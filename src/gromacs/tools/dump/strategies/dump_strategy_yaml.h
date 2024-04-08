#ifndef GMX_TOOLS_DUMP_STRATEGY_YAML_H
#define GMX_TOOLS_DUMP_STRATEGY_YAML_H

#define XX 0
#define YY 1
#define ZZ 2

#include <stack>

#include "gromacs/utility/basedefinitions.h"
#include "gromacs/utility/real.h"
#include "gromacs/tools/dump/dump_strategy.h"
#include "gromacs/tools/dump/components/dump_components_yaml.h"

class YamlStrategy : public DumpStrategy {
private:
    std::stack<YamlComponent*> componentsStack;
public:
    YamlStrategy(FILE* fp) {
        YamlComponent* root = new YamlRootComponent(fp);
        componentsStack.push(root);
    }

    ~YamlStrategy() {
        while (componentsStack.size() > 1) {
            componentsStack.pop();
        }
        if (!componentsStack.empty()) {
            YamlComponent* comp = componentsStack.top();
            componentsStack.pop();
            delete comp;
        }
    }

    bool available(const void* p, const std::string title) override;

    void pr_filename(const std::string filename) override;
    void pr_title(const std::string title) override;
    void pr_title_i(const std::string title, int i) override;
    void pr_title_n(const std::string title, int n) override;
    void pr_title_nxn(const std::string title, int n1, int n2) override;
    void close_section() override;

    void pr_named_value(const std::string name, const Value& value) override;
    void pr_named_value_short_format(const std::string name, const Value& value) override
    {
        pr_named_value(name, value);
    }
    void pr_named_value_scientific(const std::string name, const real& value) override
    {}
    void pr_attribute(const std::string name, const Value& value) override;
    void pr_attribute_quoted(const std::string name, const std::string& value) override;
    void pr_vec_attributes(const std::string title, int i, const char** names, char** values, int n) override;

    void pr_ivec(const std::string title, const int vec[], int n) override;
    void pr_rvec(const std::string title, const real vec[], int n) override;

    void pr_ivecs(const std::string title, const ivec vec[], int n) override;
    void pr_rvecs(const std::string title, const rvec vec[], int n) override;

    void pr_ivec_row(const std::string title, const int vec[], int n) override;
    void pr_rvec_row(const std::string title, const real vec[], int n) override;
    void pr_dvec_row(const std::string title, const double vec[], int n) override;
    void pr_svec_row(const std::string title, const char* vec[], int n) override;

    void pr_ivec_block(const std::string title, const int vec[], int n) override;
    
    void pr_matrix(const std::string title, const rvec* m) override;

    void pr_kvtree(const gmx::KeyValueTreeObject kvTree) override;
    
    void pr_tpx_header(const TpxFileHeader* sh) override;
    
    // TODO: add pr vec of coords
    // Tu pokracuj
    void pr_sim_annealing(const std::string title, const SimulatedAnnealing sa[], int n) override {}
    void pr_grps(gmx::ArrayRef<const AtomGroupIndices> grps, const char* const* const* grpname) override {}
    void pr_group_stats(gmx::EnumerationArray<SimulationAtomGroupType, std::vector<int>>* gcount) override {}
    void pr_moltype(const int moltype, const std::string moltypeName) override {}

    void pr_anneal_points(const std::string title, const float vec[], int n) override {}
    void pr_functypes(const std::vector<int>& functype, const int n, const std::vector<t_iparams>& iparams) override {}

    void pr_grp_opt_agg(
        const rvec acceleration[], const int ngacc,
        const ivec nFreeze[], const int ngfrz,
        const int egp_flags[], const int ngener
    ) override {}

    void pr_atoms(const t_atoms* atoms) override {}
    
    void pr_list_i(const std::string title, const int index, gmx::ArrayRef<const int> list) override {}

    void pr_interaction_list(const std::string& title, const t_functype* functypes, const InteractionList& ilist, const t_iparams* iparams) override {}

    void pr_groups(const SimulationGroups& groups) override {}

    void pr_resinfo(const t_resinfo* resinfo, int n) override {}
    
    void pr_cmap(const gmx_cmap_t* cmap_grid) override {}

};

#endif
