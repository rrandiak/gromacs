#include "sim_groups.h"

void SimulationGroupsBuilder::build(DumpStrategy* strategy)
{
    strategy->pr_grps(groups.groups, groups.groupNames.data());

    strategy->pr_title_i("grpname", groups.groupNames.size());
    const char* names[] = {"name"};
    for (size_t i = 0; i < groups.groupNames.size(); i++)
    {
        strategy->pr_vec_attributes("grpname", i, names, groups.groupNames.data()[i], 1);
    }
    strategy->close_section();

    strategy->pr_groups(groups);

    // pr_indent(fp, indent);
    // fprintf(fp, "groups          ");
    // for (const auto group : gmx::EnumerationWrapper<SimulationAtomGroupType>{})
    // {
    //     printf(" %5.5s", shortName(group));
    // }
    // printf("\n");

    // pr_indent(fp, indent);
    // fprintf(fp, "allocated       ");
    // int nat_max = 0;
    // for (auto group : keysOf(groups.groups))
    // {
    //     printf(" %5d", groups.numberOfGroupNumbers(group));
    //     nat_max = std::max(nat_max, groups.numberOfGroupNumbers(group));
    // }
    // printf("\n");

    // if (nat_max == 0)
    // {
    //     pr_indent(fp, indent);
    //     fprintf(fp, "groupnr[%5s] =", "*");
    //     for (auto gmx_unused group : keysOf(groups.groups))
    //     {
    //         fprintf(fp, "  %3d ", 0);
    //     }
    //     fprintf(fp, "\n");
    // }
    // else
    // {
    //     for (int i = 0; i < nat_max; i++)
    //     {
    //         pr_indent(fp, indent);
    //         fprintf(fp, "groupnr[%5d] =", i);
    //         for (auto group : keysOf(groups.groups))
    //         {
    //             fprintf(fp,
    //                     "  %3d ",
    //                     !groups.groupNumbers[group].empty() ? groups.groupNumbers[group][i] : 0);
    //         }
    //         fprintf(fp, "\n");
    //     }
    // }
}