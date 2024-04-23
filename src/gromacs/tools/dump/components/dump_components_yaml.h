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
class YamlInlineObject;

class YamlComponent : public DumpComponent {
protected:
    ValueComponent valueComponent;

public:
    YamlComponent(FILE* fp, int indent) : DumpComponent(fp, indent), valueComponent(fp) {}
    ~YamlComponent() = default;

    virtual YamlObjectComponent* addYamlObject(const std::string& name);
    virtual YamlObjectComponent* addYamlObject(const std::string& name, const int index);
    YamlObjectComponent* addYamlObject(const std::string& name, std::string param);
    YamlArrayComponent* addYamlArray(const std::string& name);
    YamlInlineArrayComponent* addYamlInlineArray();
    YamlInlineArrayComponent* addYamlInlineArray(const std::string& name);
    virtual YamlInlineObject* addYamlInlineObject();
    virtual YamlInlineObject* addYamlInlineObject(const std::string& name);
    void printKeyValue(const std::string& key, const Value& value);
    void printArrayValue(const Value& value);
};

class YamlObjectComponent : public YamlComponent {
public:
    YamlObjectComponent(FILE* fp, int indent) : YamlComponent(fp, indent) {}
    ~YamlObjectComponent() = default;
};

class YamlArrayComponent : public YamlComponent {
public:
    YamlArrayComponent(FILE* fp, int indent) : YamlComponent(fp, indent) {}
    ~YamlArrayComponent() = default;

    virtual YamlObjectComponent* addYamlObject(const std::string& name);
    virtual YamlObjectComponent* addYamlObject(const std::string& name, const int index);
    virtual YamlInlineObject* addYamlInlineObject();
    virtual YamlInlineObject* addYamlInlineObject(const std::string& name);
};

class YamlInlineArrayComponent : public YamlComponent {
private:
    bool isEmpty = true;
public:
    YamlInlineArrayComponent(FILE* fp, int indent) : YamlComponent(fp, indent) {}
    ~YamlInlineArrayComponent()
    {
        fprintf(fp, "]");
    }

    void printValue(const Value& value)
    {
        if (isEmpty)
        {
            isEmpty = false;
        }
        else
        {
            fprintf(fp, ", ");
        }
        valueComponent.printValue(value);
    }
};

class YamlInlineObject : public YamlComponent {
private:
    bool isEmpty = true;
public:
    YamlInlineObject(FILE* fp, int indent) : YamlComponent(fp, indent) {}
    ~YamlInlineObject()
    {
        fprintf(fp, "}");
    }

    YamlInlineArrayComponent* addYamlInlineArray();
    YamlInlineArrayComponent* addYamlInlineArray(std::string name);

    void printKeyValue(const std::string name, const Value& value)
    {
        if (isEmpty)
        {
            isEmpty = false;
        }
        else
        {
            fprintf(fp, ", ");
        }
        fprintf(fp, "%s: ", name.c_str());
        valueComponent.printValue(value);
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
