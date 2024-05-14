#include "gromacs/tools/dump/components/dump_components_text.h"

TextComponent* TextComponent::addSection(const std::string& name, const int withNextIndent) {
    fprintf(fp, "\n%*s%s:", indent, "", name.c_str());
    return new TextComponent(fp, indent + withNextIndent);
}

TextComponent* TextComponent::addSection(const std::string& name) {
    return addSection(name, TEXT_INDENT);
}
    
TextComponent* TextComponent::addSectionWithIndent(const std::string& name, int indent_)
{
    fprintf(fp, "\n%*s%s:", indent_, "", name.c_str());
    return new TextComponent(fp, indent_ + TEXT_INDENT);
}

TextComponent* TextComponent::addEmptySection()
{
    return new TextComponent(fp, indent + indentValue);
}

void TextComponent::printLeaf(const std::string& key, const Value& value, const int align) {
    fprintf(fp, "\n%*s%-*s = ", indent, "", align, key.c_str());
    valueComponent.printValue(value);
}

void TextComponent::printAttribute(const std::string name, const Value& value)
{
    fprintf(fp, "\n%*s%s=", indent, "", name.c_str());
    valueComponent.printValue(value);
}
    
void TextComponent::addFormattedTextLeaf(const char* format, ...) {
    fprintf(fp, "\n%*s", indent, "");

    va_list args;
    va_start(args, format);
    
    vfprintf(fp, format, args);
    
    va_end(args);
}

void TextComponent::printFormattedText(const char* format, ...) {
    va_list args;
    va_start(args, format);
    
    vfprintf(fp, format, args);
    
    va_end(args);
}

void TextComponent::printList(std::string title, gmx::ArrayRef<const int> list)
{
    int size = fprintf(fp, "\n%*s%s={", indent, "", title.c_str()) - 1;

    bool isFirst = true;
    for (const int item : list)
    {
        if (!isFirst)
        {
            size += fprintf(fp, ", ");
        }
        if ((size) > (USE_WIDTH))
        {
            fprintf(fp, "\n");
            size = fprintf(fp, "%*s", indent + indentValue, "");
        }
        size += fprintf(fp, "%d", item);
        isFirst = false;
    }
    fprintf(fp, "}");
}
