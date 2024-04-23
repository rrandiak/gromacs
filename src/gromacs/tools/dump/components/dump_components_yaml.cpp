#include "gromacs/tools/dump/components/dump_components_yaml.h"

YamlObjectComponent* YamlComponent::addYamlObject(const std::string& name)
{
    fprintf(fp, "\n%*s%s:", indent, "", name.c_str());
    return new YamlObjectComponent(fp, indent + YAML_INDENT);
}

YamlObjectComponent* YamlComponent::addYamlObject(const std::string& name, const int index)
{
    fprintf(fp, "\n%*s%s: %d", indent, "", name.c_str(), index);
    return new YamlObjectComponent(fp, indent + YAML_INDENT);
}

YamlObjectComponent* YamlComponent::addYamlObject(const std::string& name, const std::string param)
{
    fprintf(fp, "\n%*s%s: %s", indent, "", name.c_str(), param.c_str());
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

YamlInlineObject* YamlComponent::addYamlInlineObject()
{
    fprintf(fp, "\n%*s{", indent, "");
    return new YamlInlineObject(fp, indent);
}

YamlInlineObject* YamlComponent::addYamlInlineObject(const std::string& name)
{
    fprintf(fp, "\n%*s%s: {", indent, "", name.c_str());
    return new YamlInlineObject(fp, indent);
}

void YamlComponent::printKeyValue(const std::string& key, const Value& value)
{
    fprintf(fp, "\n%*s%s: ", indent, "", key.c_str());
    valueComponent.printValue(value);
}

void YamlComponent::printArrayValue(const Value& value)
{
    fprintf(fp, "\n%*s- ", indent + YAML_INDENT, "");
    valueComponent.printValue(value);
}

YamlObjectComponent* YamlArrayComponent::addYamlObject(const std::string& name)
{
    fprintf(fp, "\n%*s- %s:", indent, "", name.c_str());
    return new YamlObjectComponent(fp, indent + YAML_INDENT);
}

YamlObjectComponent* YamlArrayComponent::addYamlObject(const std::string& name, const int index)
{
    fprintf(fp, "\n%*s- %s: %d", indent, "", name.c_str(), index);
    return new YamlObjectComponent(fp, indent + YAML_INDENT);
}

YamlInlineArrayComponent* YamlInlineObject::addYamlInlineArray()
{
    fprintf(fp, isEmpty ? "[" : ", [");
    return new YamlInlineArrayComponent(fp, indent);
}

YamlInlineArrayComponent* YamlInlineObject::addYamlInlineArray(std::string name)
{
    fprintf(fp, isEmpty ? "%s: [" : ", %s: [", name.c_str());
    return new YamlInlineArrayComponent(fp, indent);
}

YamlInlineObject* YamlArrayComponent::addYamlInlineObject()
{
    fprintf(fp, "\n%*s- {", indent, "");
    return new YamlInlineObject(fp, indent);
}

YamlInlineObject* YamlArrayComponent::addYamlInlineObject(const std::string& name)
{
    fprintf(fp, "\n%*s- %s: {", indent, "", name.c_str());
    return new YamlInlineObject(fp, indent);
}
