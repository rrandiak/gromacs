#include "molblock.h"

void MolblockBuilder::build(DumpStrategy* strategy)
{
    strategy->pr_count("molblock", molblock.size());
    strategy->pr_title_list("molblock");
    for (size_t mb = 0; mb < molblock.size(); mb++)
    {
        strategy->pr_title_i("molblock", mb);
        // fprintf(fp, "%-20s = %d \"%s\"\n", "moltype", molb->type, *(molt[molb->type].name));
        strategy->pr_moltype(molblock[mb].type, *(moltype[molblock[mb].type].name));
        strategy->pr_count("molecules", molblock[mb].nmol);
        strategy->pr_count("posres_xA", molblock[mb].posres_xA.size());
        if (!molblock[mb].posres_xA.empty())
        {
            strategy->pr_rvecs("posres_xA", as_rvec_array(molblock[mb].posres_xA.data()), molblock[mb].posres_xA.size());
        }
        strategy->pr_count("posres_xB", molblock[mb].posres_xB.size());
        if (!molblock[mb].posres_xB.empty())
        {
            strategy->pr_rvecs("posres_xB", as_rvec_array(molblock[mb].posres_xB.data()), molblock[mb].posres_xB.size());
        }
        strategy->close_section();
    }
    strategy->close_list();
}