#include "tpr_director.h"

#include <utility>

#include "gromacs/fileio/gmxfio.h"

#include "gromacs/tools/dump/builders/grp_opts.h"
#include "gromacs/tools/dump/builders/grp_stats.h"
#include "gromacs/tools/dump/builders/inputrec.h"
#include "gromacs/tools/dump/builders/qm_opts.h"
#include "gromacs/tools/dump/builders/topology.h"
#include "gromacs/tools/dump/builders/tpx_header.h"

TprDirector::TprDirector(const char* filename, const char* mdpFilename, gmx_bool bOriginalInputrec, std::vector<TprSection> sections)
    : filename(filename), mdpFilename(mdpFilename), bOriginalInputrec(bOriginalInputrec), sections(std::move(sections))
{
    tpx = readTpxHeader(filename, true);

    read_tpx_state(filename, tpx.bIr ? &ir : nullptr, &state, tpx.bTop ? &mtop : nullptr);
    if (tpx.bIr && !bOriginalInputrec) {
        gmx::MDModules().adjustInputrecBasedOnModules(&ir);
    }
}

void TprDirector::build(DumpStrategy* strategy) {

    if (mdpFilename && tpx.bIr) {
        FILE* gp = gmx_fio_fopen(mdpFilename, "w");
        pr_inputrec(gp, 0, nullptr, &ir, TRUE);
        gmx_fio_fclose(gp);
        return;
    }

    if (strategy->available(&tpx, filename))
    {
        strategy->pr_filename(filename);

        buildSection(TprSection::InputRec, strategy);

        buildSection(TprSection::QmOptions, strategy);

        buildSection(TprSection::GroupOptions, strategy);

        buildSection(TprSection::TpxHeader, strategy);
            
        buildSection(TprSection::Topology, strategy);

        buildSection(TprSection::Box, strategy);
        buildSection(TprSection::BoxRel, strategy);
        buildSection(TprSection::BoxV, strategy);
        buildSection(TprSection::PresPrev, strategy);
        buildSection(TprSection::SvirPrev, strategy);
        buildSection(TprSection::FvirPrev, strategy);
        /* leave nosehoover_xi in for now to match the tpr version */
        buildSection(TprSection::NosehooverXI, strategy);
        /*strategy->pr_dvec_row("nosehoover_vxi",state.nosehoover_vxi,state.ngtc);*/
        /*strategy->pr_dvec_row("therm_integral",state.therm_integral,state.ngtc);*/
        buildSection(TprSection::X, strategy);
        buildSection(TprSection::V, strategy);
    }

    buildSection(TprSection::GroupStats, strategy);

    if (strategy->available(&tpx, filename))
    {
        strategy->close_section();
    }
}

void TprDirector::buildSection(TprSection section, DumpStrategy* strategy)
{
    if (!sections.empty())
    {
        auto found = std::find(sections.begin(), sections.end(), section);
        if (found == sections.end())
        {
            return;
        }
    }

    switch (section) {
        case TprSection::InputRec:
            InputRecBuilder(tpx.bIr ? &ir : nullptr).build(strategy);
            break;
        case TprSection::QmOptions:
            QmOptionsBuilder(&ir).build(strategy);
            break;
        case TprSection::GroupOptions:
            GroupOptionsBuilder(&(ir.opts)).build(strategy);
            break;
        case TprSection::TpxHeader:
            TpxHeaderBuilder(&tpx).build(strategy);
            break;
        case TprSection::Topology:
            TopologyBuilder(&mtop).build(strategy);
            break;
        case TprSection::Box:
            strategy->pr_rvecs("box", tpx.bBox ? state.box : nullptr, DIM);
            break;
        case TprSection::BoxRel:
            strategy->pr_rvecs("box_rel", tpx.bBox ? state.box_rel : nullptr, DIM);
            break;
        case TprSection::BoxV:
            strategy->pr_rvecs("boxv", tpx.bBox ? state.boxv : nullptr, DIM);
            break;
        case TprSection::PresPrev:
            strategy->pr_rvecs("pres_prev", tpx.bBox ? state.pres_prev : nullptr, DIM);
            break;
        case TprSection::SvirPrev:
            strategy->pr_rvecs("svir_prev", tpx.bBox ? state.svir_prev : nullptr, DIM);
            break;
        case TprSection::FvirPrev:
            strategy->pr_rvecs("fvir_prev", tpx.bBox ? state.fvir_prev : nullptr, DIM);
            break;
        case TprSection::NosehooverXI:
            strategy->pr_dvec_row("nosehoover_xi", state.nosehoover_xi.data(), state.ngtc);
            break;
        case TprSection::X:
            strategy->pr_rvecs("x", tpx.bX ? state.x.rvec_array() : nullptr, state.numAtoms());
            break;
        case TprSection::V:
            strategy->pr_rvecs("v", tpx.bV ? state.v.rvec_array() : nullptr, state.numAtoms());
            break;
        case TprSection::GroupStats:
            GroupStatsBuilder(&mtop.groups, mtop.natoms).build(strategy);
            break;
        default:
            break;
    }
}
