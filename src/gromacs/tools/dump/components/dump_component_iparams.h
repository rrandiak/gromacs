#ifndef GMX_TOOLS_DUMP_COMPONENT_IPARAMS_H
#define GMX_TOOLS_DUMP_COMPONENT_IPARAMS_H

#include <vector>
#include <variant>
#include <string>

#include "gromacs/topology/idef.h"

using Value = std::variant<std::string, int, int64_t, long unsigned int, double, real, gmx_bool>;

struct KeyFormatValue {
    const char *key;
    const char* format;
    const Value& value;
};

// TODO: as component that takes function that prints this

std::vector<KeyFormatValue> getIParamsValues(t_functype ftype, const t_iparams &iparams);

#endif
