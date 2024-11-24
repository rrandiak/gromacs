#ifndef GMX_TOOLS_DUMP_BUILDER_H
#define GMX_TOOLS_DUMP_BUILDER_H

#include "gromacs/tools/reprs/repr_formatter.h"

class ObjectRepr
{
public:
    virtual ~ObjectRepr()                       = default;
    virtual void build(ReprFormatter* strategy) = 0;
};

#endif
