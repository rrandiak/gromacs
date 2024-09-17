#ifndef GMX_TOOLS_DUMP_COMPONENTS_JSON_H
#define GMX_TOOLS_DUMP_COMPONENTS_JSON_H

#include "gromacs/tools/dump/dump_component.h"
#include "gromacs/tools/dump/components/value_component.h"

#define JSON_INDENT 2
    
class JsonObjectComponent;
class JsonArrayComponent;

// JSON Base Component
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
    virtual ~JsonDumpComponent() { closeLastChild(); }

    virtual JsonObjectComponent* addJsonObject(const std::string& name);
    virtual JsonArrayComponent* addJsonArray(const std::string& name);
    void printKeyValue(const std::string& key, const Value& value);

    // These methods should only work on JSON arrays
    virtual JsonObjectComponent* addJsonObject();
    virtual JsonArrayComponent* addJsonArray();
    virtual void printValue([[maybe_unused]] const Value& value);
};

// JSON Root Component
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

// JSON Object Component
class JsonObjectComponent : public JsonDumpComponent {
public:
    JsonObjectComponent(FILE* fp, int indent) : JsonDumpComponent(fp, indent) {}

    virtual ~JsonObjectComponent() {
        closeLastChild();
        if (isEmpty)
        {
            fprintf(fp, "}");
        }
        else
        {
            fprintf(fp, "\n%*s}", indent, "");
        }
    }
};

// JSON Array Component
class JsonArrayComponent : public JsonDumpComponent {
public:
    JsonArrayComponent(FILE* fp, int indent) : JsonDumpComponent(fp, indent) {}

    virtual ~JsonArrayComponent() {
        closeLastChild();
        fprintf(fp, "\n%*s]", indent, "");
    }

    JsonObjectComponent* addJsonObject(const std::string& name) override;
    JsonArrayComponent* addJsonArray(const std::string& name) override;
    using JsonDumpComponent::printKeyValue;

    JsonObjectComponent* addJsonObject() override;
    JsonArrayComponent* addJsonArray() override;
    void printValue(const Value& value) override;
};

#endif
