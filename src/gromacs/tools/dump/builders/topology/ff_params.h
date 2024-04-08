#ifndef GMX_TOOLS_DUMP_BUILDER_FEP_H
#define GMX_TOOLS_DUMP_BUILDER_FEP_H

#include "gromacs/mdtypes/inputrec.h"
#include "gromacs/utility/strconvert.h"

#include "gromacs/tools/dump/dump_builder.h"

class FFParamsBuilder : DumpBuilder {
private:
    const gmx_ffparams_t& ffparams;

public:
    FFParamsBuilder(const gmx_ffparams_t& ffparams) : ffparams(ffparams) {}
    void build(DumpStrategy* strategy) override;
};

#endif

