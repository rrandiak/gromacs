#ifndef GMX_TOOLS_DUMP_BUILDER_GRP_STATS_H
#define GMX_TOOLS_DUMP_BUILDER_GRP_STATS_H

#include "gromacs/mdtypes/inputrec.h"

#include "gromacs/tools/dump/dump_builder.h"

class GroupStatsBuilder : public DumpBuilder {
private:
    const SimulationGroups* groups;
    const int natoms;

public:
    GroupStatsBuilder(const SimulationGroups* groups, int natoms)
        : groups(groups), natoms(natoms) {}
    void build(DumpStrategy* strategy) override;
};

#endif
