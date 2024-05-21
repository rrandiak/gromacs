#ifndef GMX_TOOLS_DUMP_BUILDERS_TOPOLOGY_H
#define GMX_TOOLS_DUMP_BUILDERS_TOPOLOGY_H

#include "gromacs/tools/dump/dump_builder.h"
#include "gromacs/tools/dump/dump_strategy.h"

#include "gromacs/topology/mtop_util.h"
#include "gromacs/topology/topology.h"

class TopologyBuilder : DumpBuilder {
private:
    gmx_mtop_t* mtop;

public:
    TopologyBuilder(gmx_mtop_t* mtop) : mtop(mtop) {}
    void build(DumpStrategy* strategy) override;
};

class MTopBuilder : DumpBuilder {
private:
    const gmx_mtop_t* mtop;

public:
    MTopBuilder(const gmx_mtop_t* mtop) : mtop(mtop) {}
    void build(DumpStrategy* strategy) override;
};

class TopBuilder : DumpBuilder {
private:
    const t_topology* top;

public:
    TopBuilder(const t_topology* top) : top(top) {}
    void build(DumpStrategy* strategy) override;
};

class AtomsBuilder : DumpBuilder {
private:
    const t_atoms* atoms;

public:
    AtomsBuilder(const t_atoms* atoms) : atoms(atoms) {}
    void build(DumpStrategy* strategy) override;
};

class FFParamsBuilder : DumpBuilder {
private:
    const gmx_ffparams_t& ffparams;

public:
    FFParamsBuilder(const gmx_ffparams_t& ffparams) : ffparams(ffparams) {}
    void build(DumpStrategy* strategy) override;
};

class ListOfListsBuilder: DumpBuilder {
private:
    const std::string& title;
    const gmx::ListOfLists<int>& lists;

public:
    ListOfListsBuilder(const std::string& title, const gmx::ListOfLists<int>& lists)
        : title(title), lists(lists) {}
    void build(DumpStrategy* strategy) override;
};

class MolblockBuilder : DumpBuilder {
private:
    const std::vector<gmx_molblock_t>& molblock;
    const std::vector<gmx_moltype_t>& moltype;

public:
    MolblockBuilder(const std::vector<gmx_molblock_t>& molblock, const std::vector<gmx_moltype_t>& moltype)
        : molblock(molblock), moltype(moltype) {}
    void build(DumpStrategy* strategy) override;
};

class MoltypeBuilder : DumpBuilder {
private:
    const gmx_moltype_t* moltype;
    const int index;
    const gmx_ffparams_t& ffparams;

public:
    MoltypeBuilder(const gmx_moltype_t* moltype, int index, const gmx_ffparams_t& ffparams)
        : moltype(moltype), index(index), ffparams(ffparams) {}
    void build(DumpStrategy* strategy) override;
};

class SimulationGroupsBuilder : DumpBuilder {
private:
    const SimulationGroups groups;

public:
    SimulationGroupsBuilder(const SimulationGroups& groups) : groups(groups) {}
    void build(DumpStrategy* strategy) override;
};

#endif
