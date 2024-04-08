#ifndef GMX_TOOLS_DUMP_BUILDERS_TOPOLOGY_H
#define GMX_TOOLS_DUMP_BUILDERS_TOPOLOGY_H

#include "gromacs/tools/dump/dump_builder.h"
#include "gromacs/tools/dump/dump_strategy.h"

#include "gromacs/topology/mtop_util.h"
#include "gromacs/topology/topology.h"
#include "gromacs/utility/strconvert.h"

#include "topology/ff_params.h"
#include "topology/molblock.h"
#include "topology/moltype.h"
#include "topology/sim_groups.h"

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



class DumpBuilderCmapGrid: DumpBuilder {
private:
    const gmx_cmap_t cmap;
public:
    DumpBuilderCmapGrid(const gmx_cmap_t& cmap) : cmap(cmap) {}

    void build(DumpStrategy* strategy) override;
};



#endif
