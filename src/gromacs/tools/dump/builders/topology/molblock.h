#ifndef GMX_TOOLS_DUMP_BUILDER_MOLBLOCK_H
#define GMX_TOOLS_DUMP_BUILDER_MOLBLOCK_H

#include "gromacs/mdtypes/inputrec.h"
#include "gromacs/utility/strconvert.h"

#include "gromacs/tools/dump/dump_builder.h"

class MolblockBuilder : DumpBuilder {
private:
    const std::vector<gmx_molblock_t>& molblock;
    const std::vector<gmx_moltype_t>& moltype;

public:
    MolblockBuilder(const std::vector<gmx_molblock_t>& molblock, const std::vector<gmx_moltype_t>& moltype)
        : molblock(molblock), moltype(moltype) {}
    void build(DumpStrategy* strategy) override;
};

#endif

