#include "atoms.h"

void AtomsBuilder::build(DumpStrategy* strategy)
{
    strategy->pr_title("atoms");

    if (strategy->available(atoms->atom, "atom"))
    {
        strategy->pr_title_n("atom", atoms->nr);
        for (int i = 0; (i < atoms->nr); i++)
        {
            strategy->pr_atom(&(atoms->atom[i]), i);
        }
        strategy->close_section();

        strategy->pr_title_atom_names(atoms->nr);
        const char* atomAttributeNames[] = {"name"};
        for (int i = 0; (i < atoms->nr); i++)
        {
            strategy->pr_vec_attributes("atom", i,  atomAttributeNames, atoms->atomname[i], 1);
        }
        strategy->close_section();

        strategy->pr_title_n("type", atoms->nr);
        const char* typeAttributeNames[] = {"name", "nameB"};
        for (int i = 0; (i < atoms->nr); i++)
        {
            char* values[] = {*(atoms->atomtype[i]), *(atoms->atomtypeB[i])};
            strategy->pr_vec_attributes("type", i,  typeAttributeNames, values, 2);
        }
        strategy->close_section();

        if (strategy->available(atoms->resinfo, "residue"))
        {
            strategy->pr_title_n("residue", atoms->nres);
            for (int i = 0; i < atoms->nres; i++)
            {
                strategy->pr_residue(&(atoms->resinfo[i]), i);
            }
            strategy->close_section();
        }

    }

    strategy->close_section();
}
