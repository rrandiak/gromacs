#ifndef GMX_TOOLS_DUMP_BUILDER_ROT_H
#define GMX_TOOLS_DUMP_BUILDER_ROT_H

#include "gromacs/mdtypes/inputrec.h"
#include "gromacs/utility/strconvert.h"

#include "gromacs/tools/dump/dump_builder.h"

class RotBuilder : public DumpBuilder {
private:
    const t_rot* rot;

public:
    RotBuilder(const t_rot* rot) : rot(rot) {}
    void build(DumpStrategy* strategy) override;
};

class RotGroupBuilder : public DumpBuilder {
private:
    const int index;
    const t_rotgrp* rotg;

public:
    RotGroupBuilder(const int index, const t_rotgrp* rotg) : index(index), rotg(rotg) {}
    void build(DumpStrategy* strategy) override;
};

#endif
