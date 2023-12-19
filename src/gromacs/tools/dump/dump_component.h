#ifndef GMX_TOOLS_DUMP_COMPONENT_H
#define GMX_TOOLS_DUMP_COMPONENT_H

class DumpComponent {
protected:
    FILE* fp;
    int indent = 0;
public:
    using Value = std::variant<int, double, std::string>;

    DumpComponent(FILE* fp, int indent) : fp(fp), indent(indent) {}

    virtual ~DumpComponent() = default;
};

#endif
