#ifndef GMX_TOOLS_DUMP_SETTINGS_H
#define GMX_TOOLS_DUMP_SETTINGS_H

#include <string>
#include <variant>
#include <array>
#include <vector>

#include "gromacs/utility/basedefinitions.h"
#include "gromacs/utility/real.h"

// using Value = std::variant<std::string, int, int64_t, long unsigned int, double, real, gmx_bool, std::array<real, DIM>>;
using AtomGroupIndices = std::vector<int>;
using IParamValue = std::variant<int, real>;
using Value = std::variant<std::string, int, int64_t, long unsigned int, double, real, gmx_bool, std::array<real, 3>>;

struct DumpSettings {
    gmx_bool bSysTop = FALSE;
    gmx_bool bMDPformat = FALSE;
    gmx_bool bShowNumbers = TRUE;
    gmx_bool bShowParameters = FALSE;
    gmx_bool bJsonPrettyPrint = TRUE;
};

static DumpSettings dumpSettings;

#endif
