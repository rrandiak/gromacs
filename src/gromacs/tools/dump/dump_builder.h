#ifndef GMX_TOOLS_DUMP_BUILDER_H
#define GMX_TOOLS_DUMP_BUILDER_H

class DumpBuilder {
public:
    virtual void build(DumpStrategy* strategy, FILE* outputFile) = 0;
    virtual ~DumpBuilder() = default;
};

#endif
