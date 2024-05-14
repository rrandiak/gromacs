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
#include "gromacs/tools/dump/components/dump_component_value.h"

//! Line width for text dump output.
#define LINE_WIDTH 80
//! Right margin for text dump output.
#define RMARGIN 10
//! Actual line length for text dump output.
#define USE_WIDTH ((LINE_WIDTH) - (RMARGIN))
//! Default indentation for text dump output.
#define TEXT_INDENT 3

class TextComponent : public DumpComponent {
private:
    ValueComponent valueComponent;
    int indentValue = TEXT_INDENT;
    
public:
    TextComponent(FILE* fp, int indent, int nextIndent) : DumpComponent(fp, indent), valueComponent(fp), indentValue(nextIndent) {}
    TextComponent(FILE* fp, int indent) : DumpComponent(fp, indent), valueComponent(fp) {}
    virtual ~TextComponent() = default;

    TextComponent* addSection(const std::string& name, const int withIndent);
    TextComponent* addSection(const std::string& name);
    TextComponent* addSectionWithIndent(const std::string& name, const int indent_);
    TextComponent* addEmptySection();
    void printLeaf(const std::string& key, const Value& value, const int align);
    void printAttribute(const std::string name, const Value& value); 
    void addFormattedTextLeaf(const char* format, ...);
    void printFormattedText(const char* format, ...);
    void printList(std::string title, gmx::ArrayRef<const int> list);
};

class TextRootComponent : public TextComponent {
public:
    TextRootComponent(FILE* fp) : TextComponent(fp, 0) {}
    virtual ~TextRootComponent() = default;
};

#endif
