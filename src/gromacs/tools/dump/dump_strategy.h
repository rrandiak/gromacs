#ifndef GMX_TOOLS_DUMP_STRATEGY_H
#define GMX_TOOLS_DUMP_STRATEGY_H

#include "gromacs/utility/iserializer.h"
#include "gromacs/utility/enumerationhelpers.h"
#include "gromacs/utility/arrayref.h"
#include "gromacs/utility/keyvaluetree.h"
#include "gromacs/mdtypes/md_enums.h"
#include "gromacs/topology/topology_enums.h"
#include "gromacs/topology/topology.h"
#include "gromacs/fileio/tpxio.h"

#include <cstdio>
#include <vector>

#include "gromacs/tools/dump/dump_component.h"
#include "gromacs/tools/dump/dump_settings.h"

#define DIM 3

class DumpStrategy {
public:
    gmx_bool bMDPformat = FALSE;
    gmx_bool bShowNumbers = TRUE;
    gmx_bool bShowParameters = FALSE;
public:
    virtual ~DumpStrategy() = default;

    //! Returns whether \p is available (not null), and prints a note if it is not.
    virtual bool available(const void* p, const std::string title) = 0;

    //! Prints a filename.
    virtual void pr_filename(const std::string filename) = 0;

    //! Prints a title for a dumped section.
    virtual void pr_title(const std::string title) = 0;

    //! Prints a title for a dumped section with a index suffixed.
    virtual void pr_title_i(const std::string title, int i) = 0;

    //! Prints a title for a dumped section with a dimension suffixed.
    virtual void pr_title_n(const std::string title, int n) = 0;

    //! Prints a title for a dumped section with 2D dimension suffixed (in NxM format).
    virtual void pr_title_nxm(const std::string title, int n, int m) = 0;

    virtual void close_section() = 0;

    //! Prints a named value.
    virtual void pr_named_value(const std::string name, const Value& value) = 0;
    
    virtual void pr_named_value_short_format(const std::string name, const Value& value) = 0;

    virtual void pr_named_value_scientific(const std::string name, const real& value) = 0;

    virtual void pr_attribute(const std::string name, const Value& value) = 0;

    virtual void pr_attribute_quoted(const std::string name, const std::string& value) = 0;

    virtual void pr_vec_attributes(const std::string title, int i, const char** names, char** values, int n) = 0;

    virtual void pr_residue(const t_resinfo* resinfo, int n) = 0;

    virtual void pr_ivec(const std::string title, const int vec[], int n) = 0;

    virtual void pr_rvec(const std::string title, const real vec[], int n) = 0;
    
    virtual void pr_ivecs(const std::string title, const ivec vec[], int n) = 0;

    virtual void pr_rvecs(const std::string title, const rvec vec[], int n) = 0;

    virtual void pr_ivec_row(const std::string title, const int vec[], int n) = 0;

    virtual void pr_rvec_row(const std::string title, const real vec[], int n) = 0;

    virtual void pr_dvec_row(const std::string title, const double vec[], int n) = 0;

    virtual void pr_svec_row(const std::string title, const char* vec[], int n) = 0;

    virtual void pr_sa_vec_row(const std::string title, const SimulatedAnnealing sa[], int n) = 0;
            
    virtual void pr_ap_vec_row(const std::string title, const float vec[], int n) = 0;

    virtual void pr_posrec_vec_row(const std::string title, const real vec[]) = 0;

    virtual void pr_ivec_block(const std::string title, const int vec[], int n) = 0;

    virtual void pr_matrix(const std::string title, const rvec* m) = 0;

    virtual void pr_kvtree(const gmx::KeyValueTreeObject kvTree) = 0;

    virtual void pr_moltype(const int moltype, const std::string typeName) = 0;

    virtual void pr_atom(const t_atom* atom, const int i) = 0;

    virtual void pr_grps(gmx::ArrayRef<const AtomGroupIndices> grps, const char* const* const* grpname) = 0;

    virtual void pr_grp_opt_agg(
        const rvec acceleration[], const int ngacc,
        const ivec nFreeze[], const int ngfrz,
        const int egp_flags[], const int ngener
    ) = 0;

    virtual void pr_groups(const SimulationGroups& groups) = 0;

    virtual void pr_group_stats(gmx::EnumerationArray<SimulationAtomGroupType, std::vector<int>>* gcount) = 0;

    virtual void pr_list_i(const std::string title, const int index, gmx::ArrayRef<const int> list) = 0;

    virtual void pr_iparams(t_functype ftype, const t_iparams& iparams) = 0;
    
    virtual void pr_functypes(const std::vector<int>& functype, const int n, const std::vector<t_iparams>& iparams) = 0;

    virtual void pr_interaction_list(const std::string& title, const t_functype* functypes, const InteractionList& ilist, const t_iparams* iparams) = 0;

    virtual void pr_cmap(const gmx_cmap_t* cmap_grid) = 0;
};

#endif
