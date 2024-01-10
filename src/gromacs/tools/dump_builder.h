#ifndef GMX_TOOLS_DUMP_BUILDER_H
#define GMX_TOOLS_DUMP_BUILDER_H

#include "gromacs/tools/dump_strategy.h"

class DumpBuilder {
public:
    virtual ~DumpBuilder() = default;

    virtual void build(DumpStrategy* strategy) = 0;
};

#endif
