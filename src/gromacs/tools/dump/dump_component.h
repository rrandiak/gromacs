#ifndef GMX_TOOLS_DUMP_COMPONENT_H
#define GMX_TOOLS_DUMP_COMPONENT_H

#include "gromacs/utility/real.h"
#include "gromacs/utility/basedefinitions.h"

#include <cstdio>
#include <variant>
#include <string>

class DumpComponent {
protected:
    static int indentValue;
    FILE* fp;
    int indent = 0;

public:
    using Value = std::variant<std::string, int, int64_t, long unsigned int, double, real, gmx_bool>;
    using vec = Value[];

    DumpComponent(FILE* fp, int indent) : fp(fp), indent(indent) {}

    int getIndent() const
    {
        return indent;
    }

    static void setIndent(int indent) { indentValue = indent; }
};

#endif
