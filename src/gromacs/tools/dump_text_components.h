#ifndef GMX_TOOLS_DUMP_TEXT_COMPONENTS_H
#define GMX_TOOLS_DUMP_TEXT_COMPONENTS_H

#define INDENT 3

#include <cstdio>
#include <cstdarg>
#include <string>

#include "gromacs/utility/cstringutil.h"
#include "gromacs/tools/dump_component.h"

class TextObjectComponent;
class TextArrayComponent;

class TextDumpComponent : public DumpComponent {
private:
    void printValue(const Value& value);
    
public:
    TextDumpComponent(FILE* fp, int indent) : DumpComponent(fp, indent) {}

    TextObjectComponent* addTextSection(const std::string& name);
    virtual TextObjectComponent* addTextObject(const std::string& name);
    virtual TextObjectComponent* addTextObject(const char* format, ...);
    virtual TextArrayComponent* addTextArray(const std::string& name);
    void addTextLeaf(const std::string& key, const Value& value);
    void addTextLeaf(const Value& value);
    void addTextVectorLeaf(const float values[3], int size);
    void addFormattedTextLeaf(const char* format, ...);
};

class TextObjectComponent : public TextDumpComponent {
public:
    TextObjectComponent(FILE* fp, int indent, const std::string& name)
            : TextDumpComponent(fp, indent) {
        fprintf(fp, "\n%*s%s:", indent, "", name.c_str());
    }

    TextObjectComponent(FILE* fp, int indent, const char* format, ...)
            : TextDumpComponent(fp, indent) {
        va_list args;
        va_start(args, format);

        fprintf(fp, "\n%*s", indent, "");
        vfprintf(fp, format, args);
        fprintf(fp, ":");

        va_end(args);
    }
};

class TextArrayComponent : public TextDumpComponent {
public:
    TextArrayComponent(FILE* fp, int indent, const std::string& name)
            : TextDumpComponent(fp, indent) {
        fprintf(fp, "\n%*s%s:", indent, "", name.c_str());
    }
};

class TextRootComponent : public TextDumpComponent {
public:
    TextRootComponent(FILE* fp)
            : TextDumpComponent(fp, 0) {}

    TextObjectComponent* addTextObject(const std::string& name) override {
        return new TextObjectComponent(fp, indent, name);
    }

    TextArrayComponent* addTextArray(const std::string& name) override {
        return new TextArrayComponent(fp, indent, name);
    }
};

#endif
