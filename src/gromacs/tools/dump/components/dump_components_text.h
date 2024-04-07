#ifndef GMX_TOOLS_DUMP_COMPONENTS_TEXT_H
#define GMX_TOOLS_DUMP_COMPONENTS_TEXT_H

#include <cstdio>
#include <cstdarg>
#include <string>
#include <vector>

#include "gromacs/utility/cstringutil.h"
#include "gromacs/utility/enumerationhelpers.h"
#include "gromacs/topology/topology_enums.h"
#include "gromacs/utility/arrayref.h"

#include "gromacs/tools/dump/dump_component.h"

//! Line width for text dump output.
#define LINE_WIDTH 80
//! Right margin for text dump output.
#define RMARGIN 10
//! Actual line length for text dump output.
#define USE_WIDTH ((LINE_WIDTH) - (RMARGIN))
//! Default indentation for text dump output.
// #define INDENT 3

class TextObjectComponent;
class TextArrayComponent;

class TextDumpComponent : public DumpComponent {
private:
    void printValue(const Value& value);
    
public:
    TextDumpComponent(FILE* fp, int indent) : DumpComponent(fp, indent) {}
    virtual ~TextDumpComponent() = default;

    TextObjectComponent* addTextSection(const std::string& name);
    TextDumpComponent* addEmptySection();
    virtual TextObjectComponent* addTextObject(const std::string& name);
    virtual TextObjectComponent* addTextObject(const char* format, ...);
    virtual TextObjectComponent* addTextObject(const std::string& name, int index);
    virtual TextArrayComponent* addTextArray(const std::string& name);
    void addTextLeaf(const std::string& key, const Value& value);
    void addTextLeaf(const Value& value);
    void addAlignedTextLeaf(const std::string& key, const Value& value, int align);
    void addTextVectorLeaf(const float values[3], int size);
    void addFormattedTextLeaf(const char* format, ...);
    void addGroupStats(gmx::EnumerationArray<SimulationAtomGroupType, std::vector<int>>* gcount);
    void printFilename(const std::string& filename);
    void printFormattedText(const char* format, ...);
    void addAttribute(const std::string name, const Value& value); 
    void printList(const std::string title, int index, const gmx::ArrayRef<const int> list);
};

class TextObjectComponent : public TextDumpComponent {
public:
    TextObjectComponent(FILE* fp, int indent, const std::string& name)
            : TextDumpComponent(fp, indent + indentValue) {
        fprintf(fp, "\n%*s%s:", indent, "", name.c_str());
    }

    TextObjectComponent(FILE* fp, int indent, const std::string& name, int index)
            : TextDumpComponent(fp, indent + indentValue) {
        fprintf(fp, "\n%*s%s (%d):", indent, "", name.c_str(), index);
    }

    TextObjectComponent(FILE* fp, int indent, const char* format, ...)
            : TextDumpComponent(fp, indent + indentValue) {
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
            : TextDumpComponent(fp, indent + indentValue) {
        fprintf(fp, "\n%*s%s:", indent, "", name.c_str());
    }
};

class TextRootComponent : public TextDumpComponent {
public:
    TextRootComponent(FILE* fp)
            : TextDumpComponent(fp, 0) {}
    
    virtual ~TextRootComponent() = default;

    TextObjectComponent* addTextObject(const std::string& name) override {
        return new TextObjectComponent(fp, indent, name);
    }

    TextArrayComponent* addTextArray(const std::string& name) override {
        return new TextArrayComponent(fp, indent, name);
    }
};

#endif
