#ifndef GMX_TOOLS_DUMP_STRATEGY_TEXT_H
#define GMX_TOOLS_DUMP_STRATEGY_TEXT_H

#include "gromacs/tools/dump_strategy.h"

class DumpStrategyText : public DumpStrategy {
public:
    DumpStrategyText() {}

    bool available(FILE* fp, const void* p, int indent, const char* title) override;
    int pr_title(FILE* fp, int indent, const char* title) override;
    int pr_title_n(FILE* fp, int indent, const char* title, int n) override;
    int pr_title_nxn(FILE* fp, int indent, const char* title, int n1, int n2) override;
};

#endif
