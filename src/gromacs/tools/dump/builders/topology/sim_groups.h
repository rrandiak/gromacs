#ifndef GMX_TOOLS_DUMP_BUILDER_SIM_GROUPS_H
#define GMX_TOOLS_DUMP_BUILDER_SIM_GROUPS_H

#include "gromacs/mdtypes/inputrec.h"
#include "gromacs/utility/strconvert.h"

#include "gromacs/tools/dump/dump_builder.h"

class SimulationGroupsBuilder : DumpBuilder {
private:
    const SimulationGroups groups;

public:
    SimulationGroupsBuilder(const SimulationGroups& groups) : groups(groups) {}
    void build(DumpStrategy* strategy) override;
};

#endif
