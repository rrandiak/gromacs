#ifndef GMX_TOOLS_DUMP_BUILDERS_MTOP_H
#define GMX_TOOLS_DUMP_BUILDERS_MTOP_H

#include "gromacs/tools/dump/dump_builder.h"
#include "gromacs/tools/dump/dump_strategy.h"

#include "gromacs/topology/topology.h"
#include "gromacs/utility/strconvert.h"

class DumpBuilderMTop : DumpBuilder {
private:
    const gmx_mtop_t* mtop;
    gmx_bool bShowNumbers;
    gmx_bool bShowParameters;
public:
    DumpBuilderMTop(const gmx_mtop_t* mtop,
        gmx_bool bShowNumbers,
        gmx_bool bShowParameters) : mtop(mtop), bShowNumbers(bShowNumbers), bShowParameters(bShowParameters) {}
    
    void build(DumpStrategy* strategy) override;
};

#endif
