#ifndef GMX_TOOLS_DUMP_BUILDER_ILIST_H
#define GMX_TOOLS_DUMP_BUILDER_ILIST_H

#include "gromacs/mdtypes/inputrec.h"
#include "gromacs/utility/strconvert.h"

#include "gromacs/tools/dump/dump_builder.h"

class InteractionListBuilder: DumpBuilder {
private:
    const std::string& title;
    const t_functype* functypes;
    const InteractionList& ilist;
    const t_iparams* iparams;
public:
    InteractionListBuilder(
        const std::string& title,
        const t_functype* functype,
        const InteractionList& ilist,
        const t_iparams* iparams
        ) : title(title), functypes(functype),
            ilist(ilist), iparams(iparams) {}

    void build(DumpStrategy* strategy) override;
};

#endif

