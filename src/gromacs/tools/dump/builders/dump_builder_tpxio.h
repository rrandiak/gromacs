#ifndef GROMACS_TOOLS_DUMP_BUILDER_TPXIO_H
#define GROMACS_TOOLS_DUMP_BUILDER_TPXIO_H

#include "gromacs/tools/dump/dump_builder.h"

#include "gromacs/fileio/tpxio.h"

class DumpBuilderTpxHeader: public DumpBuilder {
private:
    const TpxFileHeader* sh;

public:
    DumpBuilderTpxHeader(const TpxFileHeader* sh) : sh(sh) {}
    
    void build(DumpStrategy* strategy) override;
};

#endif
