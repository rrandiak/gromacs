#ifndef GMX_TOOLS_DUMP_BUILDER_TPR_H
#define GMX_TOOLS_DUMP_BUILDER_TPR_H

// TODO: remove?
#include "gmxpre.h"
#include "config.h"
#include "gromacs/mdrun/mdmodules.h"
#include "gromacs/fileio/tpxio.h"

#include "gromacs/fileio/gmxfio.h"
#include "gromacs/mdtypes/inputrec.h"
#include "gromacs/mdtypes/state.h"
#include "gromacs/topology/mtop_util.h"

#include "gromacs/tools/dump/dump_builder.h"
#include "gromacs/tools/dump/dump_strategy.h"


#include "gromacs/tools/dump/builders/grp_opts.h"
#include "gromacs/tools/dump/builders/grp_stats.h"
#include "gromacs/tools/dump/builders/inputrec.h"
#include "gromacs/tools/dump/builders/qm_opts.h"
#include "gromacs/tools/dump/builders/topology.h"
#include "gromacs/tools/dump/builders/tpx_header.h"

#include "gromacs/mdtypes/inputrec.h"
#include "gromacs/fileio/tpxio.h"

enum class TprSection : int
{
    InputRec,
    QmOpts,
    GroupOpts,
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

class DumpBuilderTpr : public DumpBuilder {
private:
    const char* fileName;
    gmx_bool bShowNumbers;
    gmx_bool bShowParameters;
    const char* mdpFileName;
    gmx_bool bSysTop;
    gmx_bool bOriginalInputrec;

public:
    DumpBuilderTpr(const char* fn,
                   gmx_bool bShowNumbers,
                   gmx_bool bShowParameters,
                   const char* mdpfn,
                   gmx_bool bSysTop,
                   gmx_bool bOriginalInputrec)
        : fileName(fn),
          bShowNumbers(bShowNumbers),
          bShowParameters(bShowParameters),
          mdpFileName(mdpfn),
          bSysTop(bSysTop),
          bOriginalInputrec(bOriginalInputrec) {}
    
    void build(DumpStrategy* strategy) override;

private:
    void buildSection(const char* section, DumpStrategy* strategy);
};

#endif
