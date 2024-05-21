#ifndef GMX_TOOLS_DUMP_STRATEGY_H
#define GMX_TOOLS_DUMP_STRATEGY_H

#include "gromacs/mdlib/vsite.h"
#include "gromacs/topology/block.h"
#include "gromacs/topology/idef.h"
#include "gromacs/utility/basedefinitions.h"
#include "gromacs/utility/iserializer.h"
#include "gromacs/utility/enumerationhelpers.h"
#include "gromacs/utility/arrayref.h"
#include "gromacs/utility/keyvaluetree.h"
#include "gromacs/mdtypes/md_enums.h"
#include "gromacs/mdtypes/inputrec.h"
#include "gromacs/topology/topology_enums.h"
#include "gromacs/topology/topology.h"
#include "gromacs/fileio/tpxio.h"

#include <vector>

#include "gromacs/tools/dump/dump_settings.h"

#define DIM 3

class DumpStrategy {
public:
    gmx_bool bSysTop = FALSE;
    gmx_bool bMDPformat = FALSE;
    gmx_bool bShowNumbers = TRUE;
    gmx_bool bShowParameters = FALSE;

    virtual ~DumpStrategy() = default;

    //! Returns whether \pointer is available (not null), and prints a note if it is not.
    virtual bool available(const void* pointer, std::string title) = 0;

    //! Prints a filename.
    virtual void pr_filename(std::string filename) = 0;

    //! Prints a title for a dump section.
    virtual void pr_title(std::string title) = 0;

    //! Prints a title for a dump section with a information about index.
    virtual void pr_title_i(std::string title, int index) = 0;

    //! Prints a title for a dump section with a information about dimension.
    virtual void pr_title_n(std::string title, int n) = 0;

    //! Prints a title for a dump section with a information about 2D dimension (in NxM format).
    virtual void pr_title_nxm(std::string title, int n, int m) = 0;

    //! Prints a title for a list type structure.
    virtual void pr_title_list(std::string title) = 0;

    //! Prints a title for atom names (text format specific).
    virtual void pr_title_atom_names(int n) = 0;

    //! Prints a title for pull group (text format specific).
    virtual void pr_title_pull_group(std::string title, int index) = 0;

    //! Prints a title for pull coordinates (text format specific).
    virtual void pr_title_pull_coord(std::string title, int index) = 0;

    //! Prints a title for rotation group (text format specific).
    virtual void pr_title_rot_group(std::string title, int index) = 0;

    //! Prints a title for awh (text format specific).
    virtual void pr_title_awh(std::string title) = 0;

    //! Prints a title for all lambdas section (text format specific).
    virtual void pr_title_all_lambdas(std::string title) = 0;

    //! Closes currently open section.
    virtual void close_section() = 0;

    //! Closes currently open list section.
    virtual void close_list() = 0;

    //! Prints a named value.
    virtual void pr_named_value(std::string name, const Value& value) = 0;
    
    //! Prints a named value in short format (Text specific function).
    virtual void pr_named_value_short_format(std::string name, const Value& value) = 0;

    //! Prints a named value in real scientific notation (Text specific function).
    virtual void pr_named_value_scientific(std::string name, const real& value) = 0;

    //! Prints number format as count (Specific for each format).
    virtual void pr_count(std::string name, const Value& value) = 0;

    //! Prints attribute (Text specific function).
    virtual void pr_attribute(std::string name, const Value& value) = 0;

    //! Prints attribute (Text specific function).
    virtual void pr_attribute_quoted(std::string name, const std::string& value) = 0;

    //! Prints vector of string values with their respective keys.
    virtual void pr_vec_attributes(std::string title, int index, const char** names, char** values, int n) = 0;

    //! Prints information about residue.
    virtual void pr_residue(const t_resinfo* resinfo, int n) = 0;

    //! Prints vector of ints.
    virtual void pr_ivec(std::string title, const int vec[], int n) = 0;

    //! Prints vector of reals.
    virtual void pr_rvec(std::string title, const real vec[], int n) = 0;
    
    //! Prints vector of vectors of ints.
    virtual void pr_ivecs(std::string title, const ivec vec[], int n) = 0;

    //! Prints vector of vectors of reals.
    virtual void pr_rvecs(std::string title, const rvec vec[], int n) = 0;

    //! Prints vector of ints in a row.
    virtual void pr_ivec_row(std::string title, const int vec[], int n) = 0;

    //! Prints vector of reals in a row.
    virtual void pr_rvec_row(std::string title, const real vec[], int n) = 0;

    //! Prints vector of doubles in a row.
    virtual void pr_dvec_row(std::string title, const double vec[], int n) = 0;

    //! Prints vector of strings in a row.
    virtual void pr_svec_row(std::string title, const char* vec[], int n) = 0;

    //! Prints vector of SimulatedAnnealings in a row.
    virtual void pr_sa_vec_row(std::string title, const SimulatedAnnealing sim_annealing[], int n) = 0;
            
    //! Prints vector of floats in a row (in annealing formatting).
    virtual void pr_ap_vec_row(std::string title, const float vec[], int n, int index) = 0;

    //! Prints vector of floats in a row (in annealing formatting).
    virtual void pr_posrec_vec_row(std::string title, const real vec[]) = 0;

    //! Prints t_block.
    virtual void pr_block(std::string title, const t_block* block) = 0;

    //! Prints integers in block formation.
    virtual void pr_ivec_block(std::string title, const int vec[], int n, gmx_bool bShowNumbers) = 0;

    //! Prints matrix.
    virtual void pr_matrix(std::string title, const rvec* matrix) = 0;

    //! Prints KeyValueTree recursively. Can handle all KeyValueTree values.
    virtual void pr_kvtree(gmx::KeyValueTreeObject kvTree) = 0;

    //! Prints information about mol type.
    virtual void pr_moltype(int moltype, std::string typeName) = 0;

    //! Prints information about atom (text format specific).
    virtual void pr_atom(const t_atom* atom, int index) = 0;

    //! Prints information about atom groups.
    virtual void pr_grps(gmx::ArrayRef<const AtomGroupIndices> grps, const char* const* const* grpname) = 0;

    //! Prints group options aggregation information.
    virtual void pr_grp_opt_agg(
        const rvec acceleration[], int ngacc,
        const ivec nFreeze[], int ngfrz,
        const int egp_flags[], int ngener
    ) = 0;

    //! Prints simulation groups
    virtual void pr_groups(const SimulationGroups& groups) = 0;

    //! Prints statistics about simulation groups.
    virtual void pr_group_stats(gmx::EnumerationArray<SimulationAtomGroupType, std::vector<int>>* gcount) = 0;

    //! Prints an indexed list of integers.
    virtual void pr_list_i(std::string title, int index, gmx::ArrayRef<const int> list) = 0;

    //! Prints an interaction parameter.
    virtual void pr_iparam(std::string name, std::string format, IParamValue value) = 0;

    //! Prints interaction parameter real values of a given dimension.
    virtual void pr_iparam_reals_of_dim(std::string name, std::string format, real value[3]) = 0;

    //! Ensures a line break in interaction parameters printing.
    virtual void pr_iparam_ensure_line_break() = 0;
    
    //! Prints function types with associated parameters.
    virtual void pr_functypes(const t_functype* functypes, int n, const t_iparams* iparams) = 0;

    //! Prints an interaction list with associated function types and parameters.
    virtual void pr_interaction_list(std::string title, const t_functype* functypes, const InteractionList& ilist, const t_iparams* iparams) = 0;

    //! Prints information about Coloumbic map (text format specific).
    virtual void pr_cmap(const gmx_cmap_t* cmap_grid) = 0;

    //! Prints information about separate dvdl settings (text format specific).
    virtual void pr_separate_dvdl(std::string title, bool value) = 0;
    
    //! Prints all lambda values (text format specific).
    virtual void pr_all_lambda(std::string title, const double vec[], int n_lambda) = 0;

    //! Prints initial lambda weights (text format specific1).
    virtual void pr_init_lambda_weights(std::string title, const real vec[], int n_lambda) = 0;
};

#endif
