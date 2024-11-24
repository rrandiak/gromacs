#ifndef GMX_TOOLS_DUMP_COMPONENT_IPARAMS_H
#define GMX_TOOLS_DUMP_COMPONENT_IPARAMS_H

#include "gromacs/tools/reprs/repr_formatter.h"
#include "gromacs/topology/idef.h"

void printInteractionParameters(t_functype ftype, const t_iparams& iparams, ReprFormatter* strategy);

#endif
