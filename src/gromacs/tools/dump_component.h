#ifndef GMX_TOOLS_DUMP_COMPONENT_H
#define GMX_TOOLS_DUMP_COMPONENT_H

#include "gromacs/utility/real.h"

#include <cstdio>
#include <variant>
#include <string>

class DumpComponent {
protected:
    FILE* fp;
    int indent = 0;
public:
    using Value = std::variant<std::string, int, int64_t, long unsigned int, double, real>;
    using vec = Value[];

    DumpComponent(FILE* fp, int indent) : fp(fp), indent(indent) {}
};

#endif
