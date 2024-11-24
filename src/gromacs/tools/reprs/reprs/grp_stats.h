#ifndef GMX_TOOLS_DUMP_BUILDER_GRP_STATS_H
#define GMX_TOOLS_DUMP_BUILDER_GRP_STATS_H

#include "gromacs/mdtypes/inputrec.h"
#include "gromacs/tools/reprs/object_repr.h"

class GroupStatsRepr : public ObjectRepr
{
private:
    const SimulationGroups* groups;
    const int               natoms;

public:
    GroupStatsRepr(const SimulationGroups* groups, int natoms) : groups(groups), natoms(natoms) {}
    void build(ReprFormatter* strategy) override;
};

#endif
