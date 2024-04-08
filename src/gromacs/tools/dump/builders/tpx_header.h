#ifndef GMX_TOOLS_DUMP_BUILDER_TPX_HEADER_H
#define GMX_TOOLS_DUMP_BUILDER_TPX_HEADER_H

#include "gromacs/mdtypes/inputrec.h"
#include "gromacs/utility/strconvert.h"

#include "gromacs/tools/dump/dump_builder.h"

class TpxHeaderBuilder : DumpBuilder {
private:
    const TpxFileHeader* sh;

public:
    TpxHeaderBuilder(const TpxFileHeader* sh) : sh(sh) {}
    void build(DumpStrategy* strategy) override;
};

#endif

