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

using AtomGroupIndices = std::vector<int>;
const gmx_bool bMDPformat = FALSE;

class DumpStrategy {
public:
    virtual ~DumpStrategy() = default;

    //! Returns whether \p is available (not null), and prints a note if it is not.
    virtual bool available(const void* p, const char* title) = 0;

    //! Prints a filename.
    virtual void pr_filename(const char* filename) = 0;

    //! Prints a title for a dumped section.
    virtual void pr_title(const char* title) = 0;

    //! Prints a title for a dumped section with a index suffixed.
    virtual void pr_title_i(const char* title, int i) = 0;

    //! Prints a title for a dumped section with a dimension suffixed.
    virtual void pr_title_n(const char* title, int n) = 0;

    //! Prints a title for a dumped section with 2D dimension suffixed (in NxM format).
    virtual void pr_title_nxn(const char* title, int n1, int n2) = 0;

    virtual void close_section() = 0;

    //! Prints a named value.
    virtual void pr_named_value(const char* name, const Value& value) = 0;

    virtual void pr_attribute(const char* name, const Value& value) = 0;

    // Merge with pr_attribute
    virtual void pr_attribute_quoted(const char* name, const std::string& value) = 0;

    virtual void pr_vec_attributes(const char* title, int i, const char** names, char** values, int n) = 0;

    virtual void pr_ivec(const char* title, const int vec[], int n) = 0;

    virtual void pr_rvec(const char* title, const real vec[], int n) = 0;
    
    virtual void pr_ivecs(const char* title, const ivec vec[], int n) = 0;

    virtual void pr_rvecs(const char* title, const rvec vec[], int n) = 0;

    virtual void pr_ivec_row(const char* title, const int vec[], int n) = 0;

    virtual void pr_rvec_row(const char* title, const real vec[], int n) = 0;

    virtual void pr_dvec_row(const char* title, const double vec[], int n) = 0;

    virtual void pr_svec_row(const char* title, const char* vec[], int n) = 0;

    virtual void pr_ivec_block(const char* title, const int vec[], int n) = 0;

    virtual void pr_matrix(const char* title, const rvec* m) = 0;

    virtual void pr_kvtree(const gmx::KeyValueTreeObject kvTree) = 0;

    virtual void pr_tpx_header(const TpxFileHeader* sh) = 0;

    // Tutaj pokracuj

    virtual void pr_grps(gmx::ArrayRef<const AtomGroupIndices> grps, const char* const* const* grpname) = 0;

    virtual void pr_sim_annealing(const char* title, const SimulatedAnnealing sa[], int n) = 0;

    virtual void pr_group_stats(gmx::EnumerationArray<SimulationAtomGroupType, std::vector<int>>* gcount) = 0;

    virtual void pr_moltype(const int moltype, const char* typeName) = 0;
            
    virtual void pr_anneal_points(const char* title, const float vec[], int n) = 0;

    virtual void pr_functypes(const std::vector<int>& functype, const int n, const std::vector<t_iparams>& iparams) = 0;

    virtual void pr_grp_opt_agg(
        const rvec acceleration[], const int ngacc,
        const ivec nFreeze[], const int ngfrz,
        const int egp_flags[], const int ngener
    ) = 0;

    virtual void pr_atoms(const t_atoms* atoms) = 0;

    virtual void pr_list_i(const char* title, const int index, gmx::ArrayRef<const int> list) = 0;

    virtual void pr_interaction_list(const std::string& title, const t_functype* functypes, const InteractionList& ilist, const t_iparams* iparams) = 0;

    virtual void pr_groups(const SimulationGroups& groups) = 0;

    virtual void pr_resinfo(const t_resinfo* resinfo, int n) = 0;

    virtual void pr_cmap(const gmx_cmap_t* cmap_grid) = 0;
};

#endif
