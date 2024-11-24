#ifndef GMX_TOOLS_DUMP_BUILDER_QMOPTS_H
#define GMX_TOOLS_DUMP_BUILDER_QMOPTS_H

#include "gromacs/mdtypes/awh_params.h"
#include "gromacs/mdtypes/inputrec.h"
#include "gromacs/mdtypes/pull_params.h"
#include "gromacs/tools/reprs/object_repr.h"

class QmOptionsRepr : public ObjectRepr
{
private:
    const t_inputrec* ir;

public:
    QmOptionsRepr(const t_inputrec* ir) : ir(ir) {}
    void build(ReprFormatter* strategy) override;
};

class AwhRepr : public ObjectRepr
{
private:
    const gmx::AwhParams* awhParams;

public:
    AwhRepr(gmx::AwhParams* awhParams) : awhParams(awhParams) {}

    void build(ReprFormatter* strategy) override;
};

class AwhBiasRepr : public ObjectRepr
{
private:
    const int                 index;
    const gmx::AwhBiasParams* awhBiasParams;

public:
    AwhBiasRepr(int index, const gmx::AwhBiasParams* awhBiasParams) :
        index(index), awhBiasParams(awhBiasParams)
    {
    }

    void build(ReprFormatter* strategy) override;
};

class AwhBiasDimRepr : public ObjectRepr
{
private:
    const int                index;
    const int                dimension;
    const gmx::AwhDimParams* awhDimParams;

public:
    AwhBiasDimRepr(int index, int dimension, const gmx::AwhDimParams* awhDimParams) :
        index(index), dimension(dimension), awhDimParams(awhDimParams)
    {
    }

    void build(ReprFormatter* strategy) override;
};

class ExpandedRepr : public ObjectRepr
{
private:
    const t_expanded* expanded;
    const int         n_lambda;

public:
    ExpandedRepr(const t_expanded* expanded, const int n_lambda) :
        expanded(expanded), n_lambda(n_lambda)
    {
    }
    void build(ReprFormatter* strategy) override;
};

class FepRepr : public ObjectRepr
{
private:
    const t_lambda* fep;

public:
    FepRepr(const t_lambda* fep) : fep(fep) {}
    void build(ReprFormatter* strategy) override;
};

class ImdRepr : public ObjectRepr
{
private:
    const t_IMD* imd;

public:
    ImdRepr(const t_IMD* imd) : imd(imd) {}
    void build(ReprFormatter* strategy) override;
};

class PullRepr : public ObjectRepr
{
private:
    const pull_params_t* pull;

public:
    PullRepr(const pull_params_t* pull) : pull(pull) {}
    void build(ReprFormatter* strategy) override;
};

class PullGroupRepr : public ObjectRepr
{
private:
    int                 index;
    const t_pull_group* pgrp;

public:
    PullGroupRepr(int index, const t_pull_group* pgrp) : index(index), pgrp(pgrp) {}
    void build(ReprFormatter* strategy) override;
};

class PullCoordRepr : public ObjectRepr
{
private:
    int                 index;
    const t_pull_coord* pcrd;

public:
    PullCoordRepr(int index, const t_pull_coord* pcrd) : index(index), pcrd(pcrd) {}
    void build(ReprFormatter* strategy) override;
};

class RotRepr : public ObjectRepr
{
private:
    const t_rot* rot;

public:
    RotRepr(const t_rot* rot) : rot(rot) {}
    void build(ReprFormatter* strategy) override;
};

class RotGroupRepr : public ObjectRepr
{
private:
    const int       index;
    const t_rotgrp* rotg;

public:
    RotGroupRepr(const int index, const t_rotgrp* rotg) : index(index), rotg(rotg) {}
    void build(ReprFormatter* strategy) override;
};

class SimTempRepr : public ObjectRepr
{
private:
    const t_simtemp* simtemp;
    const int        n_lambda;

public:
    SimTempRepr(const t_simtemp* simtemp, const int n_lambda) : simtemp(simtemp), n_lambda(n_lambda)
    {
    }
    void build(ReprFormatter* strategy) override;
};

class SwapRepr : public ObjectRepr
{
private:
    const t_swapcoords* swap;

public:
    SwapRepr(const t_swapcoords* swap) : swap(swap) {}
    void build(ReprFormatter* strategy) override;
};

#endif
