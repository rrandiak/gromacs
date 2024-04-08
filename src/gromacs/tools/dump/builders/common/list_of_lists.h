#ifndef GMX_TOOLS_DUMP_BUILDER_LIST_OF_LISTS_H
#define GMX_TOOLS_DUMP_BUILDER_LIST_OF_LISTS_H

#include "gromacs/mdtypes/inputrec.h"
#include "gromacs/utility/strconvert.h"

#include "gromacs/tools/dump/dump_builder.h"

class ListOfListsBuilder: DumpBuilder {
private:
    const std::string& title;
    const gmx::ListOfLists<int>& lists;

public:
    ListOfListsBuilder(const std::string& title, const gmx::ListOfLists<int>& lists)
        : title(title), lists(lists) {}
    void build(DumpStrategy* strategy) override;
};

#endif

