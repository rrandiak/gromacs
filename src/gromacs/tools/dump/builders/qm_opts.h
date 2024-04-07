#ifndef GMX_TOOLS_DUMP_BUILDER_QMOPTS_H
#define GMX_TOOLS_DUMP_BUILDER_QMOPTS_H

#include "gromacs/mdtypes/inputrec.h"

#include "gromacs/tools/dump/dump_builder.h"
#include "gromacs/tools/dump/builders/awh.h"
#include "gromacs/tools/dump/builders/expanded.h"
#include "gromacs/tools/dump/builders/fep.h"
#include "gromacs/tools/dump/builders/pull.h"
#include "gromacs/tools/dump/builders/rot.h"
#include "gromacs/tools/dump/builders/swap.h"
#include "gromacs/tools/dump/builders/dump_builders_inputrec.h"

class DumpBuilderQmOpts : public DumpBuilder {
private:
    const t_inputrec* ir;

public:
    DumpBuilderQmOpts(const t_inputrec* ir) : ir(ir) {}
    
    void build(DumpStrategy* strategy) override;
};

class ImdBuilder : public DumpBuilder {
private:
    const t_IMD* imd;

public:
    ImdBuilder(const t_IMD* imd) : imd(imd) {}
    void build(DumpStrategy* strategy) override;
};

class SimTempBuilder : public DumpBuilder {
private:
    const t_simtemp* simtemp;
    const int n_lambda;

public:
    SimTempBuilder(const t_simtemp* simtemp, const int n_lambda) : simtemp(simtemp), n_lambda(n_lambda) {}
    void build(DumpStrategy* strategy) override;
};

#endif
