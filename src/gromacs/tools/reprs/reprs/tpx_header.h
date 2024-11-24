#ifndef GMX_TOOLS_DUMP_BUILDER_TPX_HEADER_H
#define GMX_TOOLS_DUMP_BUILDER_TPX_HEADER_H

#include "gromacs/mdtypes/inputrec.h"
#include "gromacs/tools/reprs/object_repr.h"
#include "gromacs/utility/strconvert.h"

class TpxHeaderRepr : ObjectRepr
{
private:
    const TpxFileHeader* sh;

public:
    TpxHeaderRepr(const TpxFileHeader* sh) : sh(sh) {}
    void build(ReprFormatter* strategy) override;
};

#endif
