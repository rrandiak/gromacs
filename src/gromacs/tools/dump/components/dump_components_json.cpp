#include "gromacs/tools/dump/components/dump_components_json.h"

void JsonDumpComponent::printSeparator() {
    if (!isEmpty) {
        fprintf(fp, ",");
    } else {
        isEmpty = false;
    }
}

void JsonDumpComponent::printFormattedValue(const Value& value) {
    if (std::holds_alternative<std::string>(value)) {
        fprintf(fp, "\"%s\"", std::get<std::string>(value).c_str());
    } else {
        valueComponent.printValue(value);
    }
}

void JsonDumpComponent::cleanLastChild() {
    if (lastChild != nullptr) {
        delete lastChild;
        lastChild = nullptr;
    }
}

JsonObjectComponent* JsonDumpComponent::addJsonObject() {
    cleanLastChild();
    printSeparator();
    fprintf(fp, "\n%*s{", indent + JSON_INDENT, "");
    JsonObjectComponent* jsonObject = new JsonObjectComponent(fp, indent + JSON_INDENT);
    lastChild = jsonObject;
    return jsonObject;
}

JsonObjectComponent* JsonDumpComponent::addJsonObject(const std::string& name) {
    cleanLastChild();
    printSeparator();
    fprintf(fp, "\n%*s\"%s\": {", indent + JSON_INDENT, "", name.c_str());
    JsonObjectComponent* jsonObject = new JsonObjectComponent(fp, indent + JSON_INDENT);
    lastChild = jsonObject;
    return jsonObject;
}

JsonInlineArray* JsonDumpComponent::addInlineArray() {
    cleanLastChild();
    printSeparator();
    fprintf(fp, "\n%*s[", indent + JSON_INDENT, "");
    JsonInlineArray* jsonArray = new JsonInlineArray(fp, indent + JSON_INDENT);
    lastChild = jsonArray;
    return jsonArray;
}

JsonArrayComponent* JsonDumpComponent::addJsonArray(const std::string& name) {
    cleanLastChild();
    printSeparator();
    fprintf(fp, "\n%*s\"%s\": [", indent + JSON_INDENT, "", name.c_str());
    JsonArrayComponent* jsonArray = new JsonArrayComponent(fp, indent + JSON_INDENT);
    lastChild = jsonArray;
    return jsonArray;
}

JsonArrayComponent* JsonDumpComponent::addJsonArray() {
    cleanLastChild();
    printSeparator();
    fprintf(fp, "\n%*s[", indent + JSON_INDENT, "");
    JsonArrayComponent* jsonArray = new JsonArrayComponent(fp, indent + JSON_INDENT);
    lastChild = jsonArray;
    return jsonArray;
}

void JsonDumpComponent::printKeyValue(const std::string& key, const Value& value) {
    cleanLastChild();
    printSeparator();
    fprintf(fp, "\n%*s\"%s\": ", indent + JSON_INDENT, "", key.c_str());
    printFormattedValue(value);
}

void JsonDumpComponent::printValue(const Value& value) {
    cleanLastChild();
    printSeparator();
    fprintf(fp, "\n%*s", indent + JSON_INDENT, "");
    printFormattedValue(value);
}

void JsonInlineArray::printValue(const Value& value) {
    cleanLastChild();
    if (!isEmpty) {
        fprintf(fp, ", ");
    } else {
        isEmpty = false;
    }
    printFormattedValue(value);
}
