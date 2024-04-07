#ifndef GMX_TOOLS_DUMP_BUILDER_GRP_OPTS_H
#define GMX_TOOLS_DUMP_BUILDER_GRP_OPTS_H

#include "gromacs/mdtypes/inputrec.h"

#include "gromacs/tools/dump/dump_builder.h"

class GrpOptsBuilder : public DumpBuilder {
private:
    const t_grpopts* opts;

public:
    GrpOptsBuilder(const t_grpopts* opts) : opts(opts) {}
    void build(DumpStrategy* strategy) override;
};

#endif
