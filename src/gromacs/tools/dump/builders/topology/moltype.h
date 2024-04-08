#ifndef GMX_TOOLS_DUMP_BUILDER_MOLTYPE_H
#define GMX_TOOLS_DUMP_BUILDER_MOLTYPE_H

#include "gromacs/mdtypes/inputrec.h"
#include "gromacs/utility/strconvert.h"

#include "gromacs/tools/dump/dump_builder.h"
#include "gromacs/tools/dump/builders/common/list_of_lists.h"

class MoltypeBuilder : DumpBuilder {
private:
    const gmx_moltype_t* moltype;
    const int index;
    const gmx_ffparams_t& ffparams;

public:
    MoltypeBuilder(const gmx_moltype_t* moltype, int index, const gmx_ffparams_t& ffparams)
        : moltype(moltype), index(index), ffparams(ffparams) {}
    void build(DumpStrategy* strategy) override;
};

#endif

