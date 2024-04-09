#ifndef GMX_TOOLS_DUMP_BUILDER_INPUTREC_H
#define GMX_TOOLS_DUMP_BUILDER_INPUTREC_H

#include "gromacs/mdtypes/inputrec.h"
#include "gromacs/mdtypes/multipletimestepping.h"
#include "gromacs/utility/strconvert.h"

#include "gromacs/tools/dump/dump_builder.h"

// #include "gromacs/math/veccompare.h"
// #include "gromacs/math/vecdump.h"
// #include "gromacs/mdlib/vcm.h"
// #include "gromacs/mdtypes/md_enums.h"
// #include "gromacs/mdtypes/pull_params.h"
// #include "gromacs/pbcutil/pbc.h"
// #include "gromacs/utility/compare.h"
// #include "gromacs/utility/cstringutil.h"
// #include "gromacs/utility/enumerationhelpers.h"
// #include "gromacs/utility/fatalerror.h"
// #include "gromacs/utility/keyvaluetree.h"
// #include "gromacs/utility/smalloc.h"
// #include "gromacs/utility/snprintf.h"
// #include "gromacs/utility/strconvert.h"
// #include "gromacs/utility/stringutil.h"
// #include "gromacs/utility/textwriter.h"
// #include "gromacs/utility/txtdump.h"

class InputRecBuilder : public DumpBuilder {
private:
    const t_inputrec* ir;

public:
    InputRecBuilder(const t_inputrec* ir) : ir(ir) {}
    void build(DumpStrategy* strategy) override;
};

class MtsBuilder : public DumpBuilder {
private:
    const bool useMts;
    const std::vector<gmx::MtsLevel> mtsLevels;

public:
    MtsBuilder(const bool useMts, const std::vector<gmx::MtsLevel>& mtsLevels)
        : useMts(useMts), mtsLevels(mtsLevels) {}
    void build(DumpStrategy* strategy) override;
};

#endif
