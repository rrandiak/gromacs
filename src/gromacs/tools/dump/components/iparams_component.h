#ifndef GMX_TOOLS_DUMP_COMPONENT_IPARAMS_H
#define GMX_TOOLS_DUMP_COMPONENT_IPARAMS_H

#include "gromacs/topology/idef.h"

#include "gromacs/tools/dump/dump_strategy.h"

void printInteractionParameters(t_functype ftype, const t_iparams &iparams, DumpStrategy* strategy);

#endif
