#include <gtest/gtest.h>
#include <cstdio>
#include <fstream>   // Include this header for std::ifstream
#include <sstream>
#include <string>

#include "gromacs/tools/dump/components/json_components.h"
// #include "gromacs/tools/dump/components/yaml_components.h"

#include "testutils/testfilemanager.h"

namespace gmx
{

namespace test
{

// Test case for JsonDumpComponent
TEST(JsonDumpComponentTest, GeneratesCorrectJson) {
    TestFileManager fileManager;
    std::string     jsonFilename = fileManager.getTemporaryFilePath("output.json").string();
    FILE* fp = fopen(jsonFilename.c_str(), "w");

    JsonRootComponent* root = new JsonRootComponent(fp);

    // Build the JSON structure
    JsonObjectComponent* obj1 = root->addJsonObject("object1");
    obj1->printKeyValue("key1", "value1");

    JsonArrayComponent* array1 = obj1->addJsonArray("array1");
    array1->printValue("value2");
    array1->printValue("value3");

    JsonArrayComponent* array2 = obj1->addJsonArray("array2");
    array2->printValue("value4");

    JsonObjectComponent* array3_enclosure = array2->addJsonObject();
    JsonArrayComponent* array3 = array3_enclosure->addJsonArray("array3");

    JsonObjectComponent* obj2_enclosure = array3->addJsonObject();
    JsonObjectComponent* obj2 = obj2_enclosure->addJsonObject("object2");
    obj2->printKeyValue("key5", "value5");
    obj2->addJsonObject("object3");

    array3->printValue("value6");


    delete root;  // Manually delete to invoke the destructor and close the JSON structure
    fclose(fp);

    // Open file for reading
    std::ifstream jsonFile(jsonFilename);

    // Read the content into a string
    std::ostringstream buffer;
    buffer << jsonFile.rdbuf();
    std::string output = buffer.str();

    // Define the expected output
    const std::string expectedOutput =
    "{\n"
    "  \"object1\": {\n"
    "    \"key1\": \"value1\",\n"
    "    \"array1\": [\n"
    "      \"value2\",\n"
    "      \"value3\"\n"
    "    ],\n"
    "    \"array2\": [\n"
    "      \"value4\",\n"
    "      {\n"
    "        \"array3\": [\n"
    "          {\n"
    "            \"object2\": {\n"
    "              \"key5\": \"value5\",\n"
    "              \"object3\": {}\n"
    "            }\n"
    "          },\n"
    "          \"value6\"\n"
    "        ]\n"
    "      }\n"
    "    ]\n"
    "  }\n"
    "}\n";

    // Verify the output
    EXPECT_EQ(output, expectedOutput);
}

} // namespace test

} // namespace gmx
