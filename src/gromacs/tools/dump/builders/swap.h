#ifndef GMX_TOOLS_DUMP_BUILDER_SWAP_H
#define GMX_TOOLS_DUMP_BUILDER_SWAP_H

#include "gromacs/mdtypes/inputrec.h"
#include "gromacs/utility/strconvert.h"

#include "gromacs/tools/dump/dump_builder.h"

class SwapBuilder : public DumpBuilder {
private:
    const t_swapcoords* swap;

public:
    SwapBuilder(const t_swapcoords* swap) : swap(swap) {}
    void build(DumpStrategy* strategy) override;
};


#endif

