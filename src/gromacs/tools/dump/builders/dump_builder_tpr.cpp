#include "gromacs/tools/dump/builders/dump_builder_tpr.h"

void DumpBuilderTpr::build(DumpStrategy* strategy) {
    FILE*      gp;
    t_state    state;
    gmx_mtop_t mtop;
    t_topology top;

    TpxFileHeader tpx = readTpxHeader(fileName, true);
    t_inputrec    ir;

    read_tpx_state(fileName, tpx.bIr ? &ir : nullptr, &state, tpx.bTop ? &mtop : nullptr);
    if (tpx.bIr && !bOriginalInputrec) {
        gmx::MDModules().adjustInputrecBasedOnModules(&ir);
    }

    if (mdpFileName && tpx.bIr) {
        gp = gmx_fio_fopen(mdpFileName, "w");
        pr_inputrec(gp, 0, nullptr, &ir, TRUE);
        gmx_fio_fclose(gp);
    }

    if (!mdpFileName) {
        if (bSysTop) {
            top = gmx_mtop_t_to_t_topology(&mtop, false);
        }

        if (strategy->available(&tpx, fileName)) {
            strategy->pr_filename(fileName);

            DumpBuilderInputRec(tpx.bIr ? &ir : nullptr, FALSE).build(strategy);
            DumpBuilderTpxHeader(&(tpx)).build(strategy);

            if (!bSysTop)
            {
                DumpBuilderMTop(&mtop, bShowNumbers, bShowParameters).build(strategy);
            }
            else
            {
                DumpBuilderTop(&top, bShowNumbers, bShowParameters).build(strategy);
            }

            strategy->pr_rvecs("box", tpx.bBox ? state.box : nullptr, DIM);
            strategy->pr_rvecs("box_rel", tpx.bBox ? state.box_rel : nullptr, DIM);
            strategy->pr_rvecs("boxv", tpx.bBox ? state.boxv : nullptr, DIM);
            strategy->pr_rvecs("pres_prev", tpx.bBox ? state.pres_prev : nullptr, DIM);
            strategy->pr_rvecs("svir_prev", tpx.bBox ? state.svir_prev : nullptr, DIM);
            strategy->pr_rvecs("fvir_prev", tpx.bBox ? state.fvir_prev : nullptr, DIM);
            /* leave nosehoover_xi in for now to match the tpr version */
            // strategy->pr_doubles(stdout, indent, "nosehoover_xi", state.nosehoover_xi.data(), state.ngtc);
            /*strategy->pr_doubles(stdout,indent,"nosehoover_vxi",state.nosehoover_vxi,state.ngtc);*/
            /*strategy->pr_doubles(stdout,indent,"therm_integral",state.therm_integral,state.ngtc);*/
            strategy->pr_rvecs("x", tpx.bX ? state.x.rvec_array() : nullptr, state.numAtoms());
            strategy->pr_rvecs("v", tpx.bV ? state.v.rvec_array() : nullptr, state.numAtoms());

            // fprintf(stdout, "\n\n-----\n");
            // pr_title(stdout, indent, fileName);
            // pr_inputrec(stdout, 0, "inputrec", tpx.bIr ? &ir : nullptr, FALSE);
        }

        DumpBuilderGroupStats(&mtop.groups, mtop.natoms).build(strategy);
    }
}

void DumpBuilderGroupStats::build(DumpStrategy* strategy) {
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

    // FILE*      gp;
    // int        indent, atot;
    // t_state    state;
    // gmx_mtop_t mtop;
    // t_topology top;

    // TpxFileHeader tpx = readTpxHeader(fn, true);
    // t_inputrec    ir;

    // read_tpx_state(fn, tpx.bIr ? &ir : nullptr, &state, tpx.bTop ? &mtop : nullptr);
    // if (tpx.bIr && !bOriginalInputrec)
    // {
    //     MDModules().adjustInputrecBasedOnModules(&ir);
    // }

    // if (mdpFileName && tpx.bIr)
    // {
    //     gp = gmx_fio_fopen(mdpFileName, "w");
    //     pr_inputrec(gp, 0, nullptr, &ir, TRUE);
    //     gmx_fio_fclose(gp);
    // }

    // if (!mdpFileName)
    // {
    //     if (bSysTop)
    //     {
    //         top = gmx_mtop_t_to_t_topology(&mtop, false);
    //     }

    //     if (available(stdout, &tpx, 0, fn))
    //     {
    //         indent = 0;
    //         pr_title(stdout, indent, fn);
    //         pr_inputrec(stdout, 0, "inputrec", tpx.bIr ? &ir : nullptr, FALSE);

    //         pr_tpxheader(stdout, indent, "header", &(tpx));

    //         if (!bSysTop)
    //         {
    //             pr_mtop(stdout, indent, "topology", &(mtop), bShowNumbers, bShowParameters);
    //         }
    //         else
    //         {
    //             pr_top(stdout, indent, "topology", &(top), bShowNumbers, bShowParameters);
    //         }

    //         pr_rvecs(stdout, indent, "box", tpx.bBox ? state.box : nullptr, DIM);
    //         pr_rvecs(stdout, indent, "box_rel", tpx.bBox ? state.box_rel : nullptr, DIM);
    //         pr_rvecs(stdout, indent, "boxv", tpx.bBox ? state.boxv : nullptr, DIM);
    //         pr_rvecs(stdout, indent, "pres_prev", tpx.bBox ? state.pres_prev : nullptr, DIM);
    //         pr_rvecs(stdout, indent, "svir_prev", tpx.bBox ? state.svir_prev : nullptr, DIM);
    //         pr_rvecs(stdout, indent, "fvir_prev", tpx.bBox ? state.fvir_prev : nullptr, DIM);
    //         /* leave nosehoover_xi in for now to match the tpr version */
    //         pr_doubles(stdout, indent, "nosehoover_xi", state.nosehoover_xi.data(), state.ngtc);
    //         /*pr_doubles(stdout,indent,"nosehoover_vxi",state.nosehoover_vxi,state.ngtc);*/
    //         /*pr_doubles(stdout,indent,"therm_integral",state.therm_integral,state.ngtc);*/
    //         pr_rvecs(stdout, indent, "x", tpx.bX ? state.x.rvec_array() : nullptr, state.numAtoms());
    //         pr_rvecs(stdout, indent, "v", tpx.bV ? state.v.rvec_array() : nullptr, state.numAtoms());
    //     }

    //     const SimulationGroups& groups = mtop.groups;

    //     gmx::EnumerationArray<SimulationAtomGroupType, std::vector<int>> gcount;
    //     for (auto group : keysOf(gcount))
    //     {
    //         gcount[group].resize(groups.groups[group].size());
    //     }

    //     for (int i = 0; (i < mtop.natoms); i++)
    //     {
    //         for (auto group : keysOf(gcount))
    //         {
    //             gcount[group][getGroupType(groups, group, i)]++;
    //         }
    //     }
    //     printf("Group statistics\n");
    //     for (auto group : keysOf(gcount))
    //     {
    //         atot = 0;
    //         printf("%-12s: ", shortName(group));
    //         for (const auto& entry : gcount[group])
    //         {
    //             printf("  %5d", entry);
    //             atot += entry;
    //         }
    //         printf("  (total %d atoms)\n", atot);
    //     }
    // }
// }