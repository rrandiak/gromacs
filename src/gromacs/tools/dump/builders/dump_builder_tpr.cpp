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

            DumpBuilderInputRec(tpx.bIr ? &ir : nullptr).build(strategy);

            DumpBuilderQmOpts(&ir).build(strategy);

            GrpOptsBuilder(&(ir.opts)).build(strategy);

            strategy->pr_tpx_header(&tpx);

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
            strategy->pr_dvec_row("nosehoover_xi", state.nosehoover_xi.data(), state.ngtc);
            /*strategy->pr_dvec_row("nosehoover_vxi",state.nosehoover_vxi,state.ngtc);*/
            /*strategy->pr_dvec_row("therm_integral",state.therm_integral,state.ngtc);*/
            strategy->pr_rvecs("x", tpx.bX ? state.x.rvec_array() : nullptr, state.numAtoms());
            strategy->pr_rvecs("v", tpx.bV ? state.v.rvec_array() : nullptr, state.numAtoms());
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
