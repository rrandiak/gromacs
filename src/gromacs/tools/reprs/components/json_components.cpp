#include "gromacs/tools/reprs/components/json_components.h"

#include <variant>

#include "gromacs/tools/reprs/dump_settings.h"
#include "gromacs/utility/gmxassert.h"

// JSONFormatterComponent implementation
void JsonComponent::closeLastChild()
{
    if (lastChild != nullptr)
    {
        delete lastChild;
        lastChild = nullptr;
    }
}

void JsonComponent::printSeparator()
{
    closeLastChild();
    if (!isEmpty)
    {
        fprintf(fp, ",");
    }
    else
    {
        isEmpty = false;
    }
}

void JsonComponent::printFormattedValue(const Value& value)
{
    if (std::holds_alternative<std::string>(value))
    {
        fprintf(fp, "\"%s\"", std::get<std::string>(value).c_str());
    }
    else
    {
        valueComponent.printValue(value);
    }
}

JsonObjectComponent* JsonComponent::addJsonObject(const std::string& name)
{
    printSeparator();
    if (dumpSettings.bJsonPrettyPrint)
    {
        fprintf(fp, "\n%*s\"%s\": {", indent + JSON_INDENT, "", name.c_str());
    }
    else
    {
        fprintf(fp, "\"%s\":{", name.c_str());
    }
    JsonObjectComponent* jsonObject = new JsonObjectComponent(fp, indent + JSON_INDENT);
    lastChild                       = jsonObject;
    return jsonObject;
}

JsonArrayComponent* JsonComponent::addJsonArray(const std::string& name)
{
    printSeparator();
    fprintf(fp, "\n%*s\"%s\": [", indent + JSON_INDENT, "", name.c_str());
    JsonArrayComponent* jsonArray = new JsonArrayComponent(fp, indent + JSON_INDENT);
    lastChild                     = jsonArray;
    return jsonArray;
}

void JsonComponent::printKeyValue(const std::string& key, const Value& value)
{
    printSeparator();
    fprintf(fp, "\n%*s\"%s\": ", indent + JSON_INDENT, "", key.c_str());
    printFormattedValue(value);
}

JsonObjectComponent* JsonComponent::addJsonObject()
{
    GMX_ASSERT(false, "addJsonObject() should not be used in this context.");
    return nullptr;
}

JsonArrayComponent* JsonComponent::addJsonArray()
{
    GMX_ASSERT(false, "addJsonArray() should not be used in this context.");
    return nullptr;
}

void JsonComponent::printValue([[maybe_unused]] const Value& value)
{
    GMX_ASSERT(false, "printValue(const Value& value) should not be used in this context.");
}

// JsonArrayComponent implementation
JsonObjectComponent* JsonArrayComponent::addJsonObject([[maybe_unused]] const std::string& name)
{
    GMX_ASSERT(false, "addJsonObject(const std::string& name) should not be used in this context.");
    return nullptr;
}

JsonArrayComponent* JsonArrayComponent::addJsonArray([[maybe_unused]] const std::string& name)
{
    GMX_ASSERT(false, "addJsonArray(const std::string& name) should not be used in this context.");
    return nullptr;
}

JsonObjectComponent* JsonArrayComponent::addJsonObject()
{
    printSeparator();
    fprintf(fp, "\n%*s{", indent + JSON_INDENT, "");
    JsonObjectComponent* jsonObject = new JsonObjectComponent(fp, indent + JSON_INDENT);
    lastChild                       = jsonObject;
    return jsonObject;
}

JsonArrayComponent* JsonArrayComponent::addJsonArray()
{
    printSeparator();
    fprintf(fp, "\n%*s[", indent + JSON_INDENT, "");
    JsonArrayComponent* jsonArray = new JsonArrayComponent(fp, indent + JSON_INDENT);
    lastChild                     = jsonArray;
    return jsonArray;
}

void JsonArrayComponent::printValue(const Value& value)
{
    printSeparator();
    fprintf(fp, "\n%*s", indent + JSON_INDENT, "");
    printFormattedValue(value);
}
