#ifndef GMX_TOOLS_DUMP_COMPONENT_IPARAMS_H
#define GMX_TOOLS_DUMP_COMPONENT_IPARAMS_H

#include "gromacs/tools/reprs/object_repr.h"
#include "gromacs/tools/reprs/repr_formatter.h"
#include "gromacs/topology/idef.h"

class InteractionParamsRepr : public ObjectRepr
{
private:
    const t_functype ftype;
    const t_iparams& iparams;

public:
    InteractionParamsRepr(t_functype ftype, const t_iparams& iparams) :
        ftype(ftype), iparams(iparams)
    {
    }
    void build(ReprFormatter* strategy) override;
};

#endif
