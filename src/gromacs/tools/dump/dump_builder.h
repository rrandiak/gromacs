#ifndef GMX_TOOLS_DUMP_BUILDER_H
#define GMX_TOOLS_DUMP_BUILDER_H

#include <unordered_map>
#include <string>

#include "gromacs/tools/dump/dump_strategy.h"

// struct SubBuilderInfo {
//     DumpBuilder builder;
//     bool isArrayBuilder;

//     SubBuilderInfo(DumpBuilder &builder, bool isArrayBuilder)
//         : builder(builder), isArrayBuilder(isArrayBuilder) {}
// };

// struct SubBuilders {
//     std::unordered_map<std::string, SubBuilderInfo> builders;
// };

// class DumpBuilder {
// protected:
//     std::string name;
//     DumpStrategy* strategy;
//     SubBuilders subBuilders;

// public:
//     DumpBuilder(std::string name, DumpStrategy* strategy) : name(name), strategy(strategy) {};
//     virtual ~DumpBuilder() = default;
// };

class DumpBuilder {
public:
    virtual ~DumpBuilder() = default;
    virtual void build(DumpStrategy* strategy) = 0;
};

#endif
