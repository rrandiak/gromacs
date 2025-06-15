#ifndef GMX_TOOLS_DUMP_COMPONENTS_TEXT_H
#define GMX_TOOLS_DUMP_COMPONENTS_TEXT_H

#include <cstdio>

#include <string>

#include "gromacs/tools/reprs/components/value_component.h"
#include "gromacs/tools/reprs/repr_formatter.h"
#include "gromacs/utility/arrayref.h"

//! Line width for text dump output.
#define LINE_WIDTH 80
//! Right margin for text dump output.
#define RMARGIN 10
//! Actual line length for text dump output.
#define USE_WIDTH ((LINE_WIDTH) - (RMARGIN))
//! Default indentation for text dump output.
#define TEXT_INDENT 3

class TextComponent : public FormatterComponent
{
private:
    ValueComponent valueComponent;
    int            indentValue = TEXT_INDENT;

public:
    TextComponent(FILE* fp, int indent, int nextIndent) :
        FormatterComponent(fp, indent), valueComponent(fp), indentValue(nextIndent)
    {
    }
    TextComponent(FILE* fp, int indent) : FormatterComponent(fp, indent), valueComponent(fp) {}
    virtual ~TextComponent() = default;

    TextComponent* addSection(const std::string& name, int withIndent);
    TextComponent* addSection(const std::string& name);
    TextComponent* addSectionWithIndent(const std::string& name, int indent_);
    TextComponent* addEmptySection();
    void           printLeaf(const std::string& key, const Value& value, int align);
    void           printAttribute(const std::string& name, const Value& value);
    void           addFormattedTextLeaf(const char* format, ...);
    void           printFormattedText(const char* format, ...);
    void           printList(std::string& title, gmx::ArrayRef<const int> list);
};

class TextRootComponent : public TextComponent
{
public:
    TextRootComponent(FILE* fp) : TextComponent(fp, 0) {}
    virtual ~TextRootComponent() = default;
};

#endif
