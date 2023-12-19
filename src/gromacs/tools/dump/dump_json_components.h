#ifndef GMX_TOOLS_DUMP_JSON_COMPONENTS_H
#define GMX_TOOLS_DUMP_JSON_COMPONENTS_H

#include <iostream>
#include <map>
#include <vector>
#include <variant>

#define INDENT 2

class JsonObjectComponent;
class JsonArrayComponent;

class JsonDumpComponent : public DumpComponent {
private:
    bool isEmpty = true;
    JsonDumpComponent* lastChild = nullptr;

    void printSeparator();
    void printValue(const Value& value);

protected:
    void cleanLastChild();

public:
    JsonDumpComponent(FILE* fp, int indent) : DumpComponent(fp, indent) {}

    ~JsonDumpComponent() {
        cleanLastChild();
    }

    JsonObjectComponent* addJsonObject(const std::string& name);
    JsonArrayComponent* addJsonArray(const std::string& name);
    void addJsonLeaf(const std::string& key, const Value& value);
    void addJsonLeaf(const Value& value);
};

class JsonObjectComponent : public JsonDumpComponent {
public:
    JsonObjectComponent(FILE* fp, int indent, const std::string& name)
            : JsonDumpComponent(fp, indent) {
        fprintf(fp, "\n%*s\"%s\": {", indent, "", name.c_str());
    }

    ~JsonObjectComponent() {
        cleanLastChild();
        fprintf(fp, "\n%*s}", indent, "");
    }
};

class JsonArrayComponent : public JsonDumpComponent {
public:
    JsonArrayComponent(FILE* fp, int indent, const std::string& name)
            : JsonDumpComponent(fp, indent) {
        fprintf(fp, "\n%*s\"%s\": [", indent, "", name.c_str());
    }

    ~JsonArrayComponent() {
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

    ~JsonRootComponent() {
        cleanLastChild();
        fprintf(fp, "\n}\n");
    }
};

#endif
