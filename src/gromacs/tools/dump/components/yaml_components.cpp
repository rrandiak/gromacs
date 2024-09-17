#include "gromacs/tools/dump/components/yaml_components.h"

YamlObjectComponent* YamlComponent::addYamlObject(const std::string& name)
{
    fprintf(fp, "\n%*s%s:", indent, "", name.c_str());
    return new YamlObjectComponent(fp, indent + YAML_INDENT);
}

YamlObjectComponent* YamlComponent::addYamlObject(const std::string& name, const Value& value)
{
    fprintf(fp, "\n%*s%s: ", indent, "", name.c_str());
    valueComponent.printValue(value);
    return new YamlObjectComponent(fp, indent + YAML_INDENT);
}

YamlArrayComponent* YamlComponent::addYamlArray(const std::string& name)
{
    fprintf(fp, "\n%*s%s:", indent, "", name.c_str());
    return new YamlArrayComponent(fp, indent + YAML_INDENT);
}

YamlInlineArrayComponent* YamlComponent::addYamlInlineArray()
{
    fprintf(fp, "\n%*s- [", indent, "");
    return new YamlInlineArrayComponent(fp, indent);
}

YamlInlineArrayComponent* YamlComponent::addYamlInlineArray(const std::string& name)
{
    fprintf(fp, "\n%*s%s: [", indent, "", name.c_str());
    return new YamlInlineArrayComponent(fp, indent);
}

YamlInlineObjectComponent* YamlComponent::addYamlInlineObject()
{
    fprintf(fp, "\n%*s{", indent, "");
    return new YamlInlineObjectComponent(fp, indent);
}

YamlInlineObjectComponent* YamlComponent::addYamlInlineObject(const std::string& name)
{
    fprintf(fp, "\n%*s%s: {", indent, "", name.c_str());
    return new YamlInlineObjectComponent(fp, indent);
}

void YamlComponent::printKeyValue(const std::string& key, const Value& value)
{
    fprintf(fp, "\n%*s%s: ", indent, "", key.c_str());
    valueComponent.printValue(value);
}

YamlObjectComponent* YamlArrayComponent::addYamlObject(const std::string& name)
{
    fprintf(fp, "\n%*s- %s:", indent, "", name.c_str());
    return new YamlObjectComponent(fp, indent + YAML_INDENT);
}

YamlObjectComponent* YamlArrayComponent::addYamlObject(const std::string& name, const Value& value)
{
    fprintf(fp, "\n%*s- %s: ", indent, "", name.c_str());
    valueComponent.printValue(value);
    return new YamlObjectComponent(fp, indent + YAML_INDENT);
}

YamlInlineObjectComponent* YamlArrayComponent::addYamlInlineObject()
{
    fprintf(fp, "\n%*s- {", indent, "");
    return new YamlInlineObjectComponent(fp, indent);
}

YamlInlineObjectComponent* YamlArrayComponent::addYamlInlineObject(const std::string& name)
{
    fprintf(fp, "\n%*s- %s: {", indent, "", name.c_str());
    return new YamlInlineObjectComponent(fp, indent);
}

void YamlInlineArrayComponent::printValue(const Value& value)
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

YamlInlineArrayComponent* YamlInlineObjectComponent::addYamlInlineArray()
{
    fprintf(fp, isEmpty ? "[" : ", [");
    return new YamlInlineArrayComponent(fp, indent);
}

YamlInlineArrayComponent* YamlInlineObjectComponent::addYamlInlineArray(const std::string& name)
{
    fprintf(fp, isEmpty ? "%s: [" : ", %s: [", name.c_str());
    return new YamlInlineArrayComponent(fp, indent);
}

void YamlInlineObjectComponent::printKeyValue(const std::string& name, const Value& value)
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