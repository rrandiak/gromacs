#ifndef GMX_TOOLS_DUMP_BUILDER_QMOPTS_H
#define GMX_TOOLS_DUMP_BUILDER_QMOPTS_H

#include "gromacs/mdtypes/inputrec.h"

#include "gromacs/tools/dump/dump_builder.h"

#include "qm_opts/awh.h"
#include "qm_opts/expanded.h"
#include "qm_opts/fep.h"
#include "qm_opts/pull.h"
#include "qm_opts/rot.h"
#include "qm_opts/swap.h"

class QmOptionsBuilder : public DumpBuilder {
private:
    const t_inputrec* ir;

public:
    QmOptionsBuilder(const t_inputrec* ir) : ir(ir) {}
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
