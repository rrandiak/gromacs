#ifndef GMX_TOOLS_DUMP_BUILDER_EXPANDED_H
#define GMX_TOOLS_DUMP_BUILDER_EXPANDED_H

#include "gromacs/mdtypes/inputrec.h"
#include "gromacs/utility/strconvert.h"

#include "gromacs/tools/dump/dump_builder.h"

class ExpandedBuilder : public DumpBuilder {
private:
    const t_expanded* expanded;
    const int n_lambda;

public:
    ExpandedBuilder(const t_expanded* expanded, const int n_lambda) : expanded(expanded), n_lambda(n_lambda) {}
    void build(DumpStrategy* strategy) override;
};

#endif
