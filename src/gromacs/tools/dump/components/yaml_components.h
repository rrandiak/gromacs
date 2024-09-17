#ifndef GMX_TOOLS_DUMP_COMPONENTS_YAML_H
#define GMX_TOOLS_DUMP_COMPONENTS_YAML_H

#include <cstdio>
#include "gromacs/tools/dump/dump_component.h"
#include "gromacs/tools/dump/components/value_component.h"
#include "gromacs/tools/dump/dump_settings.h"

#define YAML_INDENT 2
    
class YamlObjectComponent;
class YamlArrayComponent;
class YamlInlineArrayComponent;
class YamlInlineObjectComponent;

class YamlComponent : public DumpComponent {
protected:
    ValueComponent valueComponent;

public:
    YamlComponent(FILE* fp, int indent) : DumpComponent(fp, indent), valueComponent(fp) {}
    virtual ~YamlComponent() = default;

    virtual YamlObjectComponent* addYamlObject(const std::string& name);
    virtual YamlObjectComponent* addYamlObject(const std::string& name, const Value& value);

    virtual YamlInlineObjectComponent* addYamlInlineObject();
    virtual YamlInlineObjectComponent* addYamlInlineObject(const std::string& name);

    YamlArrayComponent* addYamlArray(const std::string& name);

    YamlInlineArrayComponent* addYamlInlineArray();
    YamlInlineArrayComponent* addYamlInlineArray(const std::string& name);

    void printKeyValue(const std::string& key, const Value& value);
};

class YamlObjectComponent : public YamlComponent {
public:
    YamlObjectComponent(FILE* fp, int indent) : YamlComponent(fp, indent) {}
    virtual ~YamlObjectComponent() = default;
};

class YamlInlineObjectComponent : public YamlComponent {
private:
    bool isEmpty = true;
public:
    YamlInlineObjectComponent(FILE* fp, int indent) : YamlComponent(fp, indent) {}
    virtual ~YamlInlineObjectComponent()
    {
        fprintf(fp, "}");
    }

    YamlInlineArrayComponent* addYamlInlineArray();
    YamlInlineArrayComponent* addYamlInlineArray(const std::string& name);

    void printKeyValue(const std::string& name, const Value& value);
};

class YamlArrayComponent : public YamlComponent {
public:
    YamlArrayComponent(FILE* fp, int indent) : YamlComponent(fp, indent) {}
    virtual ~YamlArrayComponent() = default;

    YamlObjectComponent* addYamlObject(const std::string& name) override;
    YamlObjectComponent* addYamlObject(const std::string& name, const Value& value) override;

    YamlInlineObjectComponent* addYamlInlineObject() override;
    YamlInlineObjectComponent* addYamlInlineObject(const std::string& name) override;
};

class YamlInlineArrayComponent : public YamlComponent {
private:
    bool isEmpty = true;
public:
    YamlInlineArrayComponent(FILE* fp, int indent) : YamlComponent(fp, indent) {}
    virtual ~YamlInlineArrayComponent()
    {
        fprintf(fp, "]");
    }

    void printValue(const Value& value);
};

class YamlRootComponent : public YamlComponent {
public:
    YamlRootComponent(FILE* fp) : YamlComponent(fp, 0) {
        fprintf(fp, "---");
    }

    virtual ~YamlRootComponent() {
        fprintf(fp, "\n");
    }
};

#endif
