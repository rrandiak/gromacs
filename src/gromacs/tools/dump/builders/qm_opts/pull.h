#ifndef GMX_TOOLS_DUMP_BUILDER_PULL_H
#define GMX_TOOLS_DUMP_BUILDER_PULL_H

#include "gromacs/mdtypes/pull_params.h"
#include "gromacs/utility/strconvert.h"
#include "gromacs/utility/stringutil.h"

#include "gromacs/tools/dump/dump_builder.h"

class PullBuilder : public DumpBuilder {
private:
    const pull_params_t* pull;

public:
    PullBuilder(const pull_params_t* pull) : pull(pull) {}
    void build(DumpStrategy* strategy) override;
};

class PullGroupBuilder : public DumpBuilder {
private:
    int index;
    const t_pull_group* pgrp;

public:
    PullGroupBuilder(int index, const t_pull_group* pgrp) : index(index), pgrp(pgrp) {}
    void build(DumpStrategy* strategy) override;
};

class PullCoordBuilder : public DumpBuilder {
private:
    int index;
    const t_pull_coord* pcrd;

public:
    PullCoordBuilder(int index, const t_pull_coord* pcrd) : index(index), pcrd(pcrd) {}
    void build(DumpStrategy* strategy) override;
};

#endif
