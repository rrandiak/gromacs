#include <gtest/gtest.h>
#include <cstdio>
#include <sstream>
#include <string>

#include "gromacs/tools/dump/components/json_components.h"
#include "gromacs/tools/dump/components/yaml_components.h"

#include "testutils/testfilemanager.h"

namespace gmx
{

namespace test
{

// Test case for Json dump components
TEST(JsonComponentsTest, GeneratesCorrectJson) {
    TestFileManager fileManager;
    std::string     jsonFilename = fileManager.getTemporaryFilePath("output.json").string();
    FILE* fp = fopen(jsonFilename.c_str(), "w+");

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
    fflush(fp);
    fseek(fp, 0, SEEK_SET);

    // Read the content from the file
    std::ostringstream buffer;
    char readBuffer[256];
    while (fgets(readBuffer, sizeof(readBuffer), fp) != nullptr) {
        buffer << readBuffer;
    }
    // Store the file content in a string
    std::string output = buffer.str();
    // Close the file
    fclose(fp);

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

// Test case for Yaml dump components
// TEST(YamlComponentsTest, GeneratesCorrectYaml) {
//     TestFileManager fileManager;
//     std::string     jsonFilename = fileManager.getTemporaryFilePath("output.json").string();
//     FILE* fp = fopen(jsonFilename.c_str(), "w+");

//     YamlRootComponent* root = new YamlRootComponent(fp);

//     // Build the JSON structure
//     JsonObjectComponent* obj1 = root->addJsonObject("object1");
//     obj1->printKeyValue("key1", "value1");

//     JsonArrayComponent* array1 = obj1->addJsonArray("array1");
//     array1->printValue("value2");
//     array1->printValue("value3");

//     JsonArrayComponent* array2 = obj1->addJsonArray("array2");
//     array2->printValue("value4");

//     JsonObjectComponent* array3_enclosure = array2->addJsonObject();
//     JsonArrayComponent* array3 = array3_enclosure->addJsonArray("array3");

//     JsonObjectComponent* obj2_enclosure = array3->addJsonObject();
//     JsonObjectComponent* obj2 = obj2_enclosure->addJsonObject("object2");
//     obj2->printKeyValue("key5", "value5");
//     obj2->addJsonObject("object3");

//     array3->printValue("value6");


//     delete root;  // Manually delete to invoke the destructor and close the JSON structure
//     fflush(fp);
//     fseek(fp, 0, SEEK_SET);

//     // Read the content from the file
//     std::ostringstream buffer;
//     char readBuffer[256];
//     while (fgets(readBuffer, sizeof(readBuffer), fp) != nullptr) {
//         buffer << readBuffer;
//     }

//     // Store the file content in a string
//     std::string output = buffer.str();

//     // Close the file
//     fclose(fp);

//     // Define the expected output
//     const std::string expectedOutput =
//     "{\n"
//     "  \"object1\": {\n"
//     "    \"key1\": \"value1\",\n"
//     "    \"array1\": [\n"
//     "      \"value2\",\n"
//     "      \"value3\"\n"
//     "    ],\n"
//     "    \"array2\": [\n"
//     "      \"value4\",\n"
//     "      {\n"
//     "        \"array3\": [\n"
//     "          {\n"
//     "            \"object2\": {\n"
//     "              \"key5\": \"value5\",\n"
//     "              \"object3\": {}\n"
//     "            }\n"
//     "          },\n"
//     "          \"value6\"\n"
//     "        ]\n"
//     "      }\n"
//     "    ]\n"
//     "  }\n"
//     "}\n";

//     // Verify the output
//     EXPECT_EQ(output, expectedOutput);
// }

} // namespace test

} // namespace gmx
