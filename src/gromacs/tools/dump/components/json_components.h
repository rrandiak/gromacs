#ifndef GMX_TOOLS_DUMP_COMPONENTS_JSON_H
#define GMX_TOOLS_DUMP_COMPONENTS_JSON_H

#include "gromacs/tools/dump/dump_component.h"
#include "gromacs/tools/dump/components/value_component.h"

#define JSON_INDENT 2
    
class JsonObjectComponent;
class JsonArrayComponent;

class JsonDumpComponent : public DumpComponent {
protected:
    bool isEmpty = true;
    JsonDumpComponent* lastChild = nullptr;
    ValueComponent valueComponent;

    void closeLastChild();
    void printSeparator();
    void printFormattedValue(const Value& value);

public:
    JsonDumpComponent(FILE* fp, int indent) : DumpComponent(fp, indent), valueComponent(fp) {}

    virtual ~JsonDumpComponent() {
        closeLastChild();
    }

    JsonObjectComponent* addJsonObject();
    JsonObjectComponent* addJsonObject(const std::string& name);
    JsonArrayComponent* addJsonArray(const std::string& name);
    JsonArrayComponent* addJsonArray();
    void printKeyValue(const std::string& key, const Value& value);
    void printValue(const Value& value);
};

class JsonObjectComponent : public JsonDumpComponent {
public:
    JsonObjectComponent(FILE* fp, int indent) : JsonDumpComponent(fp, indent) {}

    virtual ~JsonObjectComponent() {
        closeLastChild();
        fprintf(fp, "\n%*s}", indent, "");
    }
};

class JsonArrayComponent : public JsonDumpComponent {
public:
    JsonArrayComponent(FILE* fp, int indent) : JsonDumpComponent(fp, indent) {}

    virtual ~JsonArrayComponent() {
        closeLastChild();
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
        closeLastChild();
        fprintf(fp, "\n}\n");
    }
};

#endif
