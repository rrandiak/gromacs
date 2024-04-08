#include "interaction_list.h"

void InteractionListBuilder::build(DumpStrategy* strategy)
{
    // strategy->pr_title(title.c_str());
    // strategy->pr_attribute("nr", ilist.size());

    if (ilist.empty())
    {
        return;
    }

    // strategy->pr_iatoms("iatoms");
    // indent = pr_title(fp, indent, title);
    // pr_indent(fp, indent);
    // fprintf(fp, "nr: %d\n", ilist.size());
    // if (!ilist.empty())
    // {
    //     pr_indent(fp, indent);
    //     fprintf(fp, "iatoms:\n");
    //     int j = 0;
    //     for (int i = 0; i < ilist.size();)
    //     {
    //         pr_indent(fp, indent + INDENT);
    //         const int type  = ilist.iatoms[i];
    //         const int ftype = functype[type];
    //         if (bShowNumbers)
    //         {
    //             fprintf(fp, "%d type=%d ", j, type);
    //         }
    //         j++;
    //         printf("(%s)", interaction_function[ftype].name);
    //         for (int k = 0; k < interaction_function[ftype].nratoms; k++)
    //         {
    //             fprintf(fp, " %3d", ilist.iatoms[i + 1 + k]);
    //         }
    //         if (bShowParameters)
    //         {
    //             fprintf(fp, "  ");
    //             pr_iparams(fp, ftype, iparams[type]);
    //         }
    //         fprintf(fp, "\n");
    //         i += 1 + interaction_function[ftype].nratoms;
    //     }
    // }
}
