#ifndef GMX_TOOLS_DUMP_BUILDER_INPUTREC_H
#define GMX_TOOLS_DUMP_BUILDER_INPUTREC_H

#include "gromacs/tools/dump/dump_builder.h"

#include "gromacs/mdtypes/inputrec.h"
#include "gromacs/utility/strconvert.h"
#include "gromacs/mdtypes/pull_params.h"
#include "gromacs/mdtypes/awh_params.h"

#include "gromacs/applied_forces/awh/read_params.h"
#include "gromacs/math/veccompare.h"
#include "gromacs/math/vecdump.h"
#include "gromacs/mdlib/vcm.h"
#include "gromacs/mdtypes/awh_params.h"
#include "gromacs/mdtypes/md_enums.h"
#include "gromacs/mdtypes/multipletimestepping.h"
#include "gromacs/mdtypes/pull_params.h"
#include "gromacs/pbcutil/pbc.h"
#include "gromacs/utility/compare.h"
#include "gromacs/utility/cstringutil.h"
#include "gromacs/utility/enumerationhelpers.h"
#include "gromacs/utility/fatalerror.h"
#include "gromacs/utility/keyvaluetree.h"
#include "gromacs/utility/smalloc.h"
#include "gromacs/utility/snprintf.h"
#include "gromacs/utility/strconvert.h"
#include "gromacs/utility/stringutil.h"
#include "gromacs/utility/textwriter.h"
#include "gromacs/utility/txtdump.h"

class DumpBuilderInputRec : public DumpBuilder {
private:
    const t_inputrec* ir;
    // TODO: move bMDPformat to DumpComponent as static member
    gmx_bool bMDPformat;

public:
    DumpBuilderInputRec(const t_inputrec* ir,
                        gmx_bool bMDPformat)
        : ir(ir),
          bMDPformat(bMDPformat) {}
    
    void build(DumpStrategy* strategy) override;
};

class DumpBuilderMts : public DumpBuilder {
private:
    const bool useMts;

public:
    DumpBuilderMts(const bool useMts) : useMts(useMts) {}
    
    void build(DumpStrategy* strategy) override;
};

class DumpBuilderQmOpts : public DumpBuilder {
private:
    const t_inputrec* ir;
    gmx_bool bMDPformat;

public:
    DumpBuilderQmOpts(const t_inputrec* ir, gmx_bool bMDPformat) : ir(ir), bMDPformat(bMDPformat) {}
    
    void build(DumpStrategy* strategy) override;
};

class DumpBuilderPull : public DumpBuilder {
private:
    const pull_params_t* pull;

public:
    DumpBuilderPull(const pull_params_t* pull) : pull(pull) {}
    
    void build(DumpStrategy* strategy) override;

private:
    void pr_pull_group(DumpStrategy* strategy, int g, const t_pull_group* pgrp);
    void pr_pull_coord(DumpStrategy* strategy, int c, const t_pull_coord* pcrd);
};

class DumpBuilderAwh: public DumpBuilder {
private:
    const gmx::AwhParams* awhParams;

public:
    DumpBuilderAwh(gmx::AwhParams* awhParams) : awhParams(awhParams) {}
    
    void build(DumpStrategy* strategy) override;

private:
    void pr_awh_bias_dim(DumpStrategy* strategy, const gmx::AwhDimParams& awhDimParams, const char* prefix);
    void pr_awh_bias(DumpStrategy* strategy, const gmx::AwhBiasParams& awhBiasParams, const char* prefix);
};

class DumpBuilderRot: public DumpBuilder {
private:
    const t_rot* rot;

public:
    DumpBuilderRot(const t_rot* rot) : rot(rot) {}
    
    void build(DumpStrategy* strategy) override;

private:
    void pr_rotgrp(DumpStrategy* strategy, int g, const t_rotgrp* rotg);
};

class DumpBuilderImd: public DumpBuilder {
private:
    const t_IMD* imd;

public:
    DumpBuilderImd(const t_IMD* imd) : imd(imd) {}
    
    void build(DumpStrategy* strategy) override;
};

class DumpBuilderFep: public DumpBuilder {
private:
    const t_lambda* fep;
    gmx_bool bMDPformat;

public:
    DumpBuilderFep(const t_lambda* fep, gmx_bool bMDPformat) : fep(fep), bMDPformat(bMDPformat) {}
    
    void build(DumpStrategy* strategy) override;
};

class DumpBuilderExpanded: public DumpBuilder {
private:
    const t_expanded* expanded;
    const int n_lambda;

public:
    DumpBuilderExpanded(const t_expanded* expanded, const int n_lambda) : expanded(expanded), n_lambda(n_lambda) {}
    
    void build(DumpStrategy* strategy) override;
};

class DumpBuilderSimtemp: public DumpBuilder {
private:
    const t_simtemp* simtemp;
    const int n_lambda;

public:
    DumpBuilderSimtemp(const t_simtemp* simtemp, const int n_lambda) : simtemp(simtemp), n_lambda(n_lambda) {}
    
    void build(DumpStrategy* strategy) override;
};

class DumpBuilderSwap: public DumpBuilder {
private:
    const t_swapcoords* swap;

public:
    DumpBuilderSwap(const t_swapcoords* swap) : swap(swap) {}
    
    void build(DumpStrategy* strategy) override;
};

class DumpBuilderGrpOpts: public DumpBuilder {
private:
    const t_grpopts* opts;
    gmx_bool bMDPformat;

public:
    DumpBuilderGrpOpts(const t_grpopts* opts, gmx_bool bMDPformat) : opts(opts), bMDPformat(bMDPformat) {}
    
    void build(DumpStrategy* strategy) override;
};

#endif
