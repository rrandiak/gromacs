#ifndef GMX_TOOLS_DUMP_BUILDER_FEP_H
#define GMX_TOOLS_DUMP_BUILDER_FEP_H

#include "gromacs/mdtypes/inputrec.h"
#include "gromacs/utility/strconvert.h"

#include "gromacs/tools/dump/dump_builder.h"

class FepBuilder: public DumpBuilder {
private:
    const t_lambda* fep;

public:
    FepBuilder(const t_lambda* fep) : fep(fep) {}
    void build(DumpStrategy* strategy) override;
};

#endif
