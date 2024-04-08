#ifndef GMX_TOOLS_DUMP_BUILDER_ATOMS_H
#define GMX_TOOLS_DUMP_BUILDER_ATOMS_H

#include "gromacs/mdtypes/inputrec.h"
#include "gromacs/utility/strconvert.h"

#include "gromacs/tools/dump/dump_builder.h"

class AtomsBuilder : DumpBuilder {
private:
    const t_atoms atoms;

public:
    AtomsBuilder(const t_atoms& atoms) : atoms(atoms) {}
    void build(DumpStrategy* strategy) override;
};

#endif

