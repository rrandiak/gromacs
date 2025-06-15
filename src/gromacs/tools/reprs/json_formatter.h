#ifndef GMX_TOOLS_DUMP_STRATEGY_JSON_H
#define GMX_TOOLS_DUMP_STRATEGY_JSON_H

#include <stack>

#include "gromacs/tools/reprs/components/json_components.h"
#include "gromacs/tools/reprs/repr_formatter.h"
#include "gromacs/topology/idef.h"
#include "gromacs/topology/topology.h"
#include "gromacs/utility/basedefinitions.h"
#include "gromacs/utility/real.h"

class JsonFormatter : public ReprFormatter
{
private:
    std::stack<JsonComponent*> componentsStack;

public:
    JsonFormatter(FILE* file_pointer);
    ~JsonFormatter();

    bool available(const void* pointer, std::string title) override;

    void pr_filename(std::string filename) override;
    void pr_title(std::string title) override;
    void pr_title_i(std::string title, int index) override;
    void pr_title_n(std::string title, int n) override;
    void pr_title_nxm(std::string title, int n, int m) override;
    void pr_title_list(std::string title) override;
    void pr_title_atom_names(int n) override;
    void pr_title_pull_group(std::string title, int index) override;
    void pr_title_pull_coord(std::string title, int index) override;
    void pr_title_rot_group(std::string title, int index) override;
    void pr_title_awh(std::string title) override;
    void pr_title_all_lambdas(std::string title) override;
    void close_section() override;
    void close_list() override;

    void pr_key_value(std::string name, const Value& value) override;
    void pr_key_value_short_format(std::string name, const Value& value) override;
    void pr_key_value_scientific(std::string name, const real& value) override;
    void pr_count(std::string name, const Value& value) override;

    void pr_attribute(std::string name, const Value& value) override;
    void pr_attribute_quoted(std::string name, const std::string& value) override;
    void pr_vec_attributes(std::string title, int index, const char** names, char** values, int n) override;
    void pr_residue(const t_resinfo* resinfo, int index) override;

    void pr_ivec(std::string title, const int vec[], int n) override;
    void pr_rvec(std::string title, const real vec[], int n) override;

    void pr_ivecs(std::string title, const ivec vec[], int n) override;
    void pr_rvecs(std::string title, const rvec vec[], int n) override;

    void pr_ivec_row(std::string title, const int vec[], int n) override;
    void pr_rvec_row(std::string title, const real vec[], int n) override;
    void pr_dvec_row(std::string title, const double vec[], int n) override;
    void pr_svec_row(std::string title, const char* vec[], int n) override;
    void pr_sa_vec_row(std::string title, const SimulatedAnnealing sim_annealing[], int n) override;
    void pr_ap_vec_row(std::string title, const float vec[], int n, int index) override;
    void pr_posrec_vec_row(std::string title, const real vec[]) override;

    void pr_block(std::string title, const t_block* block) override;
    void pr_ivec_block(std::string title, const int vec[], int n, gmx_bool bShowNumbers) override;

    void pr_matrix(std::string title, const rvec* matrix) override;

    void pr_kvtree(gmx::KeyValueTreeObject kvTree) override;

    void pr_moltype(int moltype, std::string moltypeName) override;
    void pr_atom(const t_atom* atom, int index) override;

    void pr_grps(gmx::ArrayRef<const AtomGroupIndices> grps, const char* const* const* grpname) override;
    void pr_grp_opt_agg(const rvec acceleration[],
                        int        ngacc,
                        const ivec nFreeze[],
                        int        ngfrz,
                        const int  egp_flags[],
                        int        ngener) override;
    void pr_groups(const SimulationGroups& groups) override;
    void pr_group_stats(gmx::EnumerationArray<SimulationAtomGroupType, std::vector<int>>* gcount) override;

    void pr_list_i(std::string title, int index, gmx::ArrayRef<const int> list) override;

    void pr_iparam(std::string name, std::string format, IParamValue value) override;
    void pr_iparam_reals_of_dim(std::string name, std::string format, real vec[3]) override;
    void pr_iparam_ensure_line_break() override {}

    void pr_functypes(const t_functype* functypes, int n, const t_iparams* iparams) override;

    void pr_interaction_list(std::string            title,
                             const t_functype*      functypes,
                             const InteractionList& ilist,
                             const t_iparams*       iparams) override;

    void pr_cmap(const gmx_cmap_t* cmap_grid) override;

    void pr_separate_dvdl(std::string title, bool value) override;

    void pr_all_lambda(std::string title, const double vec[], int n_lambda) override;

    void pr_init_lambda_weights(std::string title, const real vec[], int n_lambda) override;
};

#endif
