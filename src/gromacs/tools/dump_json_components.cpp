#include "dump_json_components.h"

void JsonDumpComponent::printSeparator() {
    if (!isEmpty) {
        fprintf(fp, ",");
    } else {
        isEmpty = false;
    }
}

void JsonDumpComponent::printValue(const Value& value) {
    if (std::holds_alternative<int>(value)) {
        fprintf(fp, "%d", std::get<int>(value));
    } else if (std::holds_alternative<double>(value)) {
        fprintf(fp, "%f", std::get<double>(value));
    } else if (std::holds_alternative<std::string>(value)) {
        fprintf(fp, "\"%s\"", std::get<std::string>(value).c_str());
    }
}

void JsonDumpComponent::cleanLastChild() {
    if (lastChild != nullptr) {
        delete lastChild;
        lastChild = nullptr;
    }
}

JsonObjectComponent* JsonDumpComponent::addJsonObject(const std::string& name) {
    cleanLastChild();
    printSeparator();
    JsonObjectComponent* jsonObject = new JsonObjectComponent(fp, indent + INDENT, name);
    lastChild = jsonObject;
    return jsonObject;
}

JsonArrayComponent* JsonDumpComponent::addJsonArray(const std::string& name) {
    cleanLastChild();
    printSeparator();
    JsonArrayComponent* jsonArray = new JsonArrayComponent(fp, indent + INDENT, name);
    lastChild = jsonArray;
    return jsonArray;
}

void JsonDumpComponent::addJsonLeaf(const std::string& key, const Value& value) {
    cleanLastChild();
    printSeparator();
    fprintf(fp, "\n%*s\"%s\": ", indent + INDENT, "", key.c_str());
    printValue(value);
}

void JsonDumpComponent::addJsonLeaf(const Value& value) {
    cleanLastChild();
    printSeparator();
    fprintf(fp, "\n%*s", indent + INDENT, "");
    printValue(value);
}
