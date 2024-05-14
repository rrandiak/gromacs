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
using Value = std::variant<std::string, int, int64_t, long unsigned int, double, real, gmx_bool, std::array<real, 3>>;

#endif
