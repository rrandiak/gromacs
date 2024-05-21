#ifndef GMX_TOOLS_DUMP_BUILDER_QMOPTS_H
#define GMX_TOOLS_DUMP_BUILDER_QMOPTS_H

#include "gromacs/mdtypes/awh_params.h"
#include "gromacs/mdtypes/inputrec.h"
#include "gromacs/mdtypes/pull_params.h"

#include "gromacs/tools/dump/dump_builder.h"

class QmOptionsBuilder : public DumpBuilder {
private:
    const t_inputrec* ir;

public:
    QmOptionsBuilder(const t_inputrec* ir) : ir(ir) {}
    void build(DumpStrategy* strategy) override;
};

class AwhBuilder: public DumpBuilder {
private:
    const gmx::AwhParams* awhParams;

public:
    AwhBuilder(gmx::AwhParams* awhParams) : awhParams(awhParams) {}
    
    void build(DumpStrategy* strategy) override;
};

class AwhBiasBuilder: public DumpBuilder {
private:
    const int index;
    const gmx::AwhBiasParams* awhBiasParams;

public:
    AwhBiasBuilder(int index, const gmx::AwhBiasParams* awhBiasParams)
        : index(index), awhBiasParams(awhBiasParams) {}
    
    void build(DumpStrategy* strategy) override;
};

class AwhBiasDimBuilder: public DumpBuilder {
private:
    const int index;
    const int dimension;
    const gmx::AwhDimParams* awhDimParams;

public:
    AwhBiasDimBuilder(int index, int dimension, const gmx::AwhDimParams* awhDimParams)
        : index(index), dimension(dimension), awhDimParams(awhDimParams) {}
    
    void build(DumpStrategy* strategy) override;
};

class ExpandedBuilder : public DumpBuilder {
private:
    const t_expanded* expanded;
    const int n_lambda;

public:
    ExpandedBuilder(const t_expanded* expanded, const int n_lambda) : expanded(expanded), n_lambda(n_lambda) {}
    void build(DumpStrategy* strategy) override;
};

class FepBuilder: public DumpBuilder {
private:
    const t_lambda* fep;

public:
    FepBuilder(const t_lambda* fep) : fep(fep) {}
    void build(DumpStrategy* strategy) override;
};

class ImdBuilder : public DumpBuilder {
private:
    const t_IMD* imd;

public:
    ImdBuilder(const t_IMD* imd) : imd(imd) {}
    void build(DumpStrategy* strategy) override;
};

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

class SimTempBuilder : public DumpBuilder {
private:
    const t_simtemp* simtemp;
    const int n_lambda;

public:
    SimTempBuilder(const t_simtemp* simtemp, const int n_lambda) : simtemp(simtemp), n_lambda(n_lambda) {}
    void build(DumpStrategy* strategy) override;
};

class SwapBuilder : public DumpBuilder {
private:
    const t_swapcoords* swap;

public:
    SwapBuilder(const t_swapcoords* swap) : swap(swap) {}
    void build(DumpStrategy* strategy) override;
};

#endif
