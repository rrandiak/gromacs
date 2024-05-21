#ifndef GMX_TOOLS_DUMP_COMPONENT_VALUE_H
#define GMX_TOOLS_DUMP_COMPONENT_VALUE_H

#include <cstdio>
#include <string>

#include "gromacs/utility/cstringutil.h"
#include "gromacs/tools/dump/dump_component.h"

class ValueComponent : public DumpComponent {
public:
    ValueComponent(FILE* fp) : DumpComponent(fp, 0) {}

    void printValue(const Value& value) {
        if (std::holds_alternative<gmx_bool>(value))
        {
            fprintf(fp, "%s", std::get<gmx_bool>(value) ? "true" : "false");
        }
        else if (std::holds_alternative<int>(value))
        {
            fprintf(fp, "%d", std::get<int>(value));
        }
        else if (std::holds_alternative<long unsigned int>(value))
        {
            fprintf(fp, "%ld", std::get<long unsigned int>(value));
        }
        else if (std::holds_alternative<double>(value))
        {
            fprintf(fp, "%g", std::get<double>(value));
        }
        else if (std::holds_alternative<std::string>(value))
        {
            fprintf(fp, "%s", std::get<std::string>(value).c_str());
        }
        else if (std::holds_alternative<int64_t>(value))
        {
            char buf[STEPSTRSIZE];
            fprintf(fp, "%s", gmx_step_str(std::get<int64_t>(value), buf));
        }
        else if (std::holds_alternative<real>(value))
        {
            fprintf(fp, "%g", std::get<real>(value));
        }
    }
    
};

#endif