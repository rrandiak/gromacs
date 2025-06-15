#ifndef GMX_TOOLS_DUMP_BUILDER_GRP_OPTS_H
#define GMX_TOOLS_DUMP_BUILDER_GRP_OPTS_H

#include "gromacs/mdtypes/inputrec.h"
#include "gromacs/tools/reprs/object_repr.h"

class GroupOptionsRepr : public ObjectRepr
{
private:
    const t_grpopts* opts;

public:
    GroupOptionsRepr(const t_grpopts* opts) : opts(opts) {}
    void build(ReprFormatter* strategy) override;
};

#endif
