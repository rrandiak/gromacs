#include "list_of_lists.h"

void ListOfListsBuilder::build(DumpStrategy* strategy)
{
    if (!strategy->available(&lists, title.c_str()))
    {
        return;
    }

    strategy->pr_title(title.c_str());
    strategy->pr_attribute("numLists", lists.size());
    strategy->pr_attribute("numElements", lists.numElements());

    strategy->pr_title_list(title.c_str());

    for (gmx::Index i = 0; i < lists.ssize(); i++)
    {
        strategy->pr_list_i(title.c_str(), i, lists[i]);
    }

    strategy->close_list();
    strategy->close_section();
}