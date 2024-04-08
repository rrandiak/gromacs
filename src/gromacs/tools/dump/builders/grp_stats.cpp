#include "grp_stats.h"

void GroupStatsBuilder::build(DumpStrategy* strategy) {
    gmx::EnumerationArray<SimulationAtomGroupType, std::vector<int>> gcount;

    for (auto group : keysOf(gcount))
    {
        gcount[group].resize(groups->groups[group].size());
    }

    for (int i = 0; (i < natoms); i++)
    {
        for (auto group : keysOf(gcount))
        {
            gcount[group][getGroupType(*groups, group, i)]++;
        }
    }

    strategy->pr_group_stats(&gcount);
}
