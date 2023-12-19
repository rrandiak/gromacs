#ifndef GMX_TOOLS_DUMP_BUILDER_TPR_H
#define GMX_TOOLS_DUMP_BUILDER_TPR_H

#include "gromacs/mdrun/mdmodules.h"
#include "gromacs/fileio/tpxio.h"

#include "dump_builder.h"
#include "gromacs/tools/dump_strategy.h"

class DumpBuilderTpr : DumpBuilder {
private:
    const char* fileName;
    gmx_bool showNumbers;
    gmx_bool showParameters;
    const char* mdpFileName;
    gmx_bool sysTop;
    gmx_bool originalInputrec;

public:
    DumpBuilderTpr(const char* fn,
                   gmx_bool bShowNumbers,
                   gmx_bool bShowParameters,
                   const char* mdpfn,
                   gmx_bool bSysTop,
                   gmx_bool bOriginalInputrec)
        : fileName(fn),
          showNumbers(bShowNumbers),
          showParameters(bShowParameters),
          mdpFileName(mdpfn),
          sysTop(bSysTop),
          originalInputrec(bOriginalInputrec) {}
    
    void build(DumpStrategy* strategy, FILE* outputFile) override;
};

#endif
