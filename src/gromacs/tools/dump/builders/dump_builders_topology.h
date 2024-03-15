#ifndef GMX_TOOLS_DUMP_BUILDERS_TOPOLOGY_H
#define GMX_TOOLS_DUMP_BUILDERS_TOPOLOGY_H

#include "gromacs/tools/dump/dump_builder.h"
#include "gromacs/tools/dump/dump_strategy.h"

#include "gromacs/topology/topology.h"
#include "gromacs/utility/strconvert.h"

class DumpBuilderFFParams: DumpBuilder {
private:
    const gmx_ffparams_t ffparams;
public:
    DumpBuilderFFParams(const gmx_ffparams_t& ffparams) : ffparams(ffparams) {}
    
    void build(DumpStrategy* strategy) override;
};

class DumpBuilderMolblock: DumpBuilder {
private:
    const std::vector<gmx_molblock_t> molblock;
    const std::vector<gmx_moltype_t> moltype;
public:
    DumpBuilderMolblock(const std::vector<gmx_molblock_t>& molblock, const std::vector<gmx_moltype_t>& moltype) : molblock(molblock), moltype(moltype) {}
    
    void build(DumpStrategy* strategy) override;
};

class DumpBuilderGroups : DumpBuilder {
private:
    const SimulationGroups groups;
    gmx_bool bShowNumbers;
public:
    DumpBuilderGroups(const SimulationGroups& groups, gmx_bool bShowNumbers) : groups(groups), bShowNumbers(bShowNumbers) {}
    
    void build(DumpStrategy* strategy) override;
};

class DumpBuilderMTop : DumpBuilder {
private:
    const gmx_mtop_t* mtop;
    gmx_bool bShowNumbers;
    gmx_bool bShowParameters;
public:
    DumpBuilderMTop(const gmx_mtop_t* mtop,
        gmx_bool bShowNumbers,
        gmx_bool bShowParameters) : mtop(mtop), bShowNumbers(bShowNumbers), bShowParameters(bShowParameters) {}
    
    void build(DumpStrategy* strategy) override;
};

class DumpBuilderTop : DumpBuilder {
private:
    const t_topology* top;
    gmx_bool bShowNumbers;
    gmx_bool bShowParameters;
public:
    DumpBuilderTop(const t_topology* top,
        gmx_bool bShowNumbers,
        gmx_bool bShowParameters) : top(top), bShowNumbers(bShowNumbers), bShowParameters(bShowParameters) {}
    
    void build(DumpStrategy* strategy) override;
};

#endif
