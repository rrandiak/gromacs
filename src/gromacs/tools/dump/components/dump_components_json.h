#ifndef GMX_TOOLS_DUMP_COMPONENTS_JSON_H
#define GMX_TOOLS_DUMP_COMPONENTS_JSON_H

#include <iostream>
#include <map>
#include <vector>
#include <variant>

#include "gromacs/tools/dump/dump_component.h"
#include "gromacs/tools/dump/components/dump_component_value.h"

#define JSON_INDENT 2
    
class JsonObjectComponent;
class JsonInlineArray;
class JsonArrayComponent;

class JsonDumpComponent : public DumpComponent {
protected:
    bool isEmpty = true;
    JsonDumpComponent* lastChild = nullptr;
    ValueComponent valueComponent;

    void printFormattedValue(const Value& value);
    void printSeparator();

protected:
    void cleanLastChild();

public:
    JsonDumpComponent(FILE* fp, int indent) : DumpComponent(fp, indent), valueComponent(fp) {}

    virtual ~JsonDumpComponent() {
        cleanLastChild();
    }

    JsonObjectComponent* addJsonObject();
    JsonObjectComponent* addJsonObject(const std::string& name);
    JsonArrayComponent* addJsonArray(const std::string& name);
    JsonArrayComponent* addJsonArray();
    JsonInlineArray* addInlineArray();
    void printKeyValue(const std::string& key, const Value& value);
    void printValue(const Value& value);
};

class JsonObjectComponent : public JsonDumpComponent {
public:
    JsonObjectComponent(FILE* fp, int indent) : JsonDumpComponent(fp, indent) {}

    virtual ~JsonObjectComponent() {
        cleanLastChild();
        fprintf(fp, "\n%*s}", indent, "");
    }
};

class JsonInlineArray : public JsonDumpComponent {
public:
    JsonInlineArray(FILE* fp, int indent)
            : JsonDumpComponent(fp, indent) {}

    virtual ~JsonInlineArray() {
        cleanLastChild();
        fprintf(fp, "]");
    }

    void printValue(const Value& value);
};

class JsonArrayComponent : public JsonDumpComponent {
public:
    JsonArrayComponent(FILE* fp, int indent) : JsonDumpComponent(fp, indent) {}

    virtual ~JsonArrayComponent() {
        cleanLastChild();
        fprintf(fp, "\n%*s]", indent, "");
    }
};

class JsonRootComponent : public JsonDumpComponent {
public:
    JsonRootComponent(FILE* fp)
            : JsonDumpComponent(fp, 0) {
        fprintf(fp, "{");
    }

    virtual ~JsonRootComponent() {
        cleanLastChild();
        fprintf(fp, "\n}\n");
    }
};

#endif
