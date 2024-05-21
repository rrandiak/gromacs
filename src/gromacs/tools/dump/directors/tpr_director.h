#ifndef GMX_TOOLS_DUMP_BUILDER_TPR_H
#define GMX_TOOLS_DUMP_BUILDER_TPR_H

#include "gromacs/mdrun/mdmodules.h"
#include "gromacs/mdtypes/state.h"

#include "gromacs/tools/dump/dump_builder.h"
#include "gromacs/tools/dump/dump_strategy.h"

enum class TprSection : int
{
    InputRec,
    QmOptions,
    GroupOptions,
    TpxHeader,
    Topology,
    Box,
    BoxRel,
    BoxV,
    PresPrev,
    SvirPrev,
    FvirPrev,
    NosehooverXI,
    X,
    V,
    GroupStats,
    Count
};

const gmx::EnumerationArray<TprSection, const char*> c_tprSectionNames = {
    "inputrec",
    "qm-opts",
    "grpopts",
    "header",
    "topology",
    "box",
    "box_rel",
    "boxv",
    "pres_prev",
    "svir_prev",
    "fvir_prev",
    "nosehoover_xi",
    "x",
    "v",
    "group_statistics"
};

//! Director for building TPR (Trajectory Parameter) files.
/*!
 * This class is responsible for orchestrating the construction of TPR files using a specified
 * build strategy.
 */
class TprDirector : public DumpBuilder {
private:
    const char* filename;
    const char* mdpFilename;
    gmx_bool bOriginalInputrec;
    std::vector<TprSection> sections;

    t_state state;
    gmx_mtop_t mtop;
    TpxFileHeader tpx;
    t_inputrec ir;

public:
    TprDirector(const char* filename, const char* mdpFilename, gmx_bool bOriginalInputrec, std::vector<TprSection> sections);
    void build(DumpStrategy* strategy) override;

private:
    void buildSection(TprSection section, DumpStrategy* strategy);
};

#endif
