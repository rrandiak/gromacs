#include "gromacs/tools/dump/components/dump_components_yaml.h"

void YamlComponent::printValue(const Value& value)
{
    if (std::holds_alternative<gmx_bool>(value)) {
        fprintf(fp, "%s", std::get<gmx_bool>(value) ? "true" : "false");
    } else if (std::holds_alternative<int>(value)) {
        fprintf(fp, "%d", std::get<int>(value));
    } else if (std::holds_alternative<long unsigned int>(value)) {
        fprintf(stderr, "Warning: long unsigned int not supported\n");
        fprintf(fp, "%ld", std::get<long unsigned int>(value));
    } else if (std::holds_alternative<double>(value)) {
        fprintf(fp, "%f", std::get<double>(value));
    } else if (std::holds_alternative<std::string>(value)) {
        fprintf(fp, "%s", std::get<std::string>(value).c_str());
    } else if (std::holds_alternative<int64_t>(value)) {
        char buf[STEPSTRSIZE];
        fprintf(fp, "%s", gmx_step_str(std::get<int64_t>(value), buf));
    } else if (std::holds_alternative<real>(value)) {
        fprintf(fp, "%g", std::get<real>(value));
    }
}


YamlObjectComponent* YamlComponent::addYamlObject(const std::string& name)
{
    fprintf(fp, "\n%*s%s:", indent, "", name.c_str());
    return new YamlObjectComponent(fp, indent + YAML_INDENT, name);
}

YamlArrayComponent* YamlComponent::addYamlArray(const std::string& name)
{
    fprintf(fp, "\n%*s%s:", indent, "", name.c_str());
    return new YamlArrayComponent(fp, indent + YAML_INDENT, name);
}

YamlInlineArrayComponent* YamlComponent::addYamlInlineArray(const std::string& name)
{
    return new YamlInlineArrayComponent(fp, indent, name);
}

void YamlComponent::printKeyValue(const std::string& key, const Value& value)
{
    fprintf(fp, "\n%*s%s: ", indent, "", key.c_str());
    printValue(value);
}

void YamlComponent::printArrayValue(const Value& value)
{
    fprintf(fp, "\n%*s- ", indent + YAML_INDENT, "");
    printValue(value);
}


YamlObjectComponent* YamlArrayComponent::addYamlObject(const std::string& name)
{
    fprintf(fp, "\n%*s- %s:", indent, "", name.c_str());
    return new YamlObjectComponent(fp, indent + YAML_INDENT, name);
}
