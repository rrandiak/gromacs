#ifndef GMX_TOOLS_DUMP_BUILDERS_TOPOLOGY_H
#define GMX_TOOLS_DUMP_BUILDERS_TOPOLOGY_H

#include "gromacs/tools/reprs/object_repr.h"
#include "gromacs/tools/reprs/repr_formatter.h"
#include "gromacs/topology/mtop_util.h"
#include "gromacs/topology/topology.h"

class TopologyRepr : ObjectRepr
{
private:
    gmx_mtop_t* mtop;

public:
    TopologyRepr(gmx_mtop_t* mtop) : mtop(mtop) {}
    void build(ReprFormatter* strategy) override;
};

class MTopRepr : ObjectRepr
{
private:
    const gmx_mtop_t* mtop;

public:
    MTopRepr(const gmx_mtop_t* mtop) : mtop(mtop) {}
    void build(ReprFormatter* strategy) override;
};

class TopRepr : ObjectRepr
{
private:
    const t_topology* top;

public:
    TopRepr(const t_topology* top) : top(top) {}
    void build(ReprFormatter* strategy) override;
};

class AtomsRepr : ObjectRepr
{
private:
    const t_atoms* atoms;

public:
    AtomsRepr(const t_atoms* atoms) : atoms(atoms) {}
    void build(ReprFormatter* strategy) override;
};

class FFParamsRepr : ObjectRepr
{
private:
    const gmx_ffparams_t& ffparams;

public:
    FFParamsRepr(const gmx_ffparams_t& ffparams) : ffparams(ffparams) {}
    void build(ReprFormatter* strategy) override;
};

class ListOfListsRepr : ObjectRepr
{
private:
    const std::string&           title;
    const gmx::ListOfLists<int>& lists;

public:
    ListOfListsRepr(const std::string& title, const gmx::ListOfLists<int>& lists) :
        title(title), lists(lists)
    {
    }
    void build(ReprFormatter* strategy) override;
};

class MolblockRepr : ObjectRepr
{
private:
    const std::vector<gmx_molblock_t>& molblock;
    const std::vector<gmx_moltype_t>&  moltype;

public:
    MolblockRepr(const std::vector<gmx_molblock_t>& molblock, const std::vector<gmx_moltype_t>& moltype) :
        molblock(molblock), moltype(moltype)
    {
    }
    void build(ReprFormatter* strategy) override;
};

class MoltypeRepr : ObjectRepr
{
private:
    const gmx_moltype_t*  moltype;
    const int             index;
    const gmx_ffparams_t& ffparams;

public:
    MoltypeRepr(const gmx_moltype_t* moltype, int index, const gmx_ffparams_t& ffparams) :
        moltype(moltype), index(index), ffparams(ffparams)
    {
    }
    void build(ReprFormatter* strategy) override;
};

class SimulationGroupsRepr : ObjectRepr
{
private:
    const SimulationGroups groups;

public:
    SimulationGroupsRepr(const SimulationGroups& groups) : groups(groups) {}
    void build(ReprFormatter* strategy) override;
};

#endif
