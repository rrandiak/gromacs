#ifndef GMX_TOOLS_DUMP_COMPONENT_H
#define GMX_TOOLS_DUMP_COMPONENT_H

#include <cstdio>
#include <variant>
#include <string>

#include "gromacs/tools/dump/dump_settings.h"

class DumpComponent {
protected:
    static int indentValue;
    FILE* fp;
    int indent = 0;

public:
    using vec = Value[];

    DumpComponent(FILE* fp, int indent) : fp(fp), indent(indent) {}
    
    int getIndent() const
    {
        return indent;
    }

    static void setIndent(int indent) { indentValue = indent; }
};

#endif
