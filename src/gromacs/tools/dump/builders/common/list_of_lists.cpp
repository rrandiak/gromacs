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
    // indent = pr_title(fp, indent, title);
    // pr_indent(fp, indent);
    // fprintf(fp, "numLists=%zu\n", lists->size());
    // pr_indent(fp, indent);
    // fprintf(fp, "numElements=%d\n", lists->numElements());

    strategy->pr_title_list(title.c_str());

    for (gmx::Index i = 0; i < lists.ssize(); i++)
    {
        // gmx::ArrayRef<const int> list = (*lists)[i];
        strategy->pr_list_i(title.c_str(), i, lists[i]);
        // int                      size = pr_indent(fp, indent);
        // gmx::ArrayRef<const int> list = (*lists)[i];
        // if (list.empty())
        // {
        //     size += fprintf(fp, "%s[%d]={", title, int(i));
        // }
        // else
        // {
        //     size += fprintf(fp, "%s[%d][num=%zu]={", title, bShowNumbers ? int(i) : -1, list.size());
        // }
        // bool isFirst = true;
        // for (const int j : list)
        // {
        //     if (!isFirst)
        //     {
        //         size += fprintf(fp, ", ");
        //     }
        //     if ((size) > (USE_WIDTH))
        //     {
        //         fprintf(fp, "\n");
        //         size = pr_indent(fp, indent + INDENT);
        //     }
        //     size += fprintf(fp, "%d", j);
        //     isFirst = false;
        // }
        // fprintf(fp, "}\n");
    }

    strategy->close_list();
    strategy->close_section();
}