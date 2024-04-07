#ifndef GMX_TOOLS_DUMP_BUILDER_AWH_H
#define GMX_TOOLS_DUMP_BUILDER_AWH_H

#include "gromacs/mdtypes/awh_params.h"
// #include "gromacs/applied_forces/awh/read_params.h"
#include "gromacs/utility/strconvert.h"
#include "gromacs/utility/stringutil.h"

#include "gromacs/tools/dump/dump_builder.h"

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

#endif
