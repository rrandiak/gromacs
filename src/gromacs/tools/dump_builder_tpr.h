#ifndef GMX_TOOLS_DUMP_BUILDER_TPR_H
#define GMX_TOOLS_DUMP_BUILDER_TPR_H

#include "gmxpre.h"

#include "dump.h"

#include "config.h"

#include "gromacs/fileio/checkpoint.h"
#include "gromacs/fileio/enxio.h"
#include "gromacs/fileio/filetypes.h"
#include "gromacs/fileio/gmxfio.h"
#include "gromacs/fileio/mtxio.h"
#include "gromacs/fileio/tngio.h"
#include "gromacs/fileio/tpxio.h"
#include "gromacs/fileio/trrio.h"
#include "gromacs/fileio/xtcio.h"
#include "gromacs/gmxpreprocess/gmxcpp.h"
#include "gromacs/math/vecdump.h"
#include "gromacs/mdrun/mdmodules.h"
#include "gromacs/mdtypes/forcerec.h"
#include "gromacs/mdtypes/inputrec.h"
#include "gromacs/mdtypes/md_enums.h"
#include "gromacs/mdtypes/state.h"
#include "gromacs/options/basicoptions.h"
#include "gromacs/options/filenameoption.h"
#include "gromacs/options/ioptionscontainer.h"
#include "gromacs/topology/mtop_util.h"
#include "gromacs/topology/topology.h"
#include "gromacs/trajectory/energyframe.h"
#include "gromacs/trajectory/trajectoryframe.h"
#include "gromacs/utility/arraysize.h"
#include "gromacs/utility/basedefinitions.h"
#include "gromacs/utility/fatalerror.h"
#include "gromacs/utility/futil.h"
#include "gromacs/utility/smalloc.h"
#include "gromacs/utility/txtdump.h"

#include "gromacs/mdrun/mdmodules.h"
#include "gromacs/fileio/tpxio.h"

#include "dump_builder.h"
#include "gromacs/tools/dump_strategy.h"

#include "gromacs/mdtypes/inputrec.h"
#include "gromacs/fileio/tpxio.h"

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
};

class DumpBuilderGroupStats : public DumpBuilder {
private:
    const SimulationGroups* groups;
    const int natoms;

public:
    DumpBuilderGroupStats(const SimulationGroups* groups, const int natoms) : groups(groups), natoms(natoms) {}

    void build(DumpStrategy* strategy) override;
};

#endif
