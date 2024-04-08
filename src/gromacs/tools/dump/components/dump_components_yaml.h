#ifndef GMX_TOOLS_DUMP_COMPONENTS_YAML_H
#define GMX_TOOLS_DUMP_COMPONENTS_YAML_H

#include <iostream>
#include <map>
#include <vector>
#include <variant>

#include "gromacs/tools/dump/dump_component.h"
#include "gromacs/tools/dump/components/dump_component_value.h"

#define YAML_INDENT 2
    
class YamlObjectComponent;
class YamlArrayComponent;
class YamlInlineArrayComponent;

class YamlComponent : public DumpComponent {

private:
    // TODO: use static ValueComponent instead
    // DumpValueComponent* valueComponent = nullptr;
    void printValue(const Value& value);

public:
    YamlComponent(FILE* fp, int indent) : DumpComponent(fp, indent) {}
    ~YamlComponent() = default;

    YamlObjectComponent* addYamlObject(const std::string& name);
    YamlArrayComponent* addYamlArray(const std::string& name);
    YamlInlineArrayComponent* addYamlInlineArray(const std::string& name);
    void printKeyValue(const std::string& key, const Value& value);
    void printArrayValue(const Value& value);
};

class YamlObjectComponent : public YamlComponent {
public:
    YamlObjectComponent(FILE* fp, int indent, const std::string& name)
            : YamlComponent(fp, indent) {}
    ~YamlObjectComponent() = default;
};

class YamlArrayComponent : public YamlComponent {
public:
    YamlArrayComponent(FILE* fp, int indent, const std::string& name)
            : YamlComponent(fp, indent) {}
    ~YamlArrayComponent() = default;

    YamlObjectComponent* addYamlObject(const std::string& name);
};

class YamlInlineArrayComponent : public YamlComponent {
public:
    YamlInlineArrayComponent(FILE* fp, int indent, const std::string& name)
            : YamlComponent(fp, indent) {
        fprintf(fp, "\n%*s\"%s\": [", indent, "", name.c_str());
    }
    ~YamlInlineArrayComponent() = default;

    void close() {
        fprintf(fp, "]");
    }
};

class YamlRootComponent : public YamlComponent {
public:
    YamlRootComponent(FILE* fp)
            : YamlComponent(fp, 0) {
        fprintf(fp, "---");
    }

    ~YamlRootComponent() {
        fprintf(fp, "\n");
    }
};

#endif
