#ifndef GMX_TOOLS_DUMP_STRATEGY_H
#define GMX_TOOLS_DUMP_STRATEGY_H

#include <cstdio>

class DumpStrategy {
public:
    virtual ~DumpStrategy() = default;

    virtual bool available(FILE* fp, const void* p, int indent, const char* title) = 0;
    virtual int pr_title(FILE* fp, int indent, const char* title) = 0;
    virtual int pr_title_n(FILE* fp, int indent, const char* title, int n) = 0;
    virtual int pr_title_nxn(FILE* fp, int indent, const char* title, int n1, int n2) = 0;
};

#endif
