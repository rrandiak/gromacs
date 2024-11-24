#include <cstdio>

#include <fstream>
#include <iostream>
#include <sstream>

#include <gtest/gtest.h>

#include "gromacs/tools/reprs/json_formatter.h"
#include "gromacs/tools/reprs/object_repr.h"
#include "gromacs/tools/reprs/repr_formatter.h"
#include "gromacs/tools/reprs/text_formatter.h"
#include "gromacs/tools/reprs/yaml_formatter.h"

#include "testutils/testfilemanager.h"

namespace gmx
{

namespace test
{

class RepresentationTest : public ::testing::Test
{
protected:
    TestFileManager fileManager;
    FILE*           fp;

    // Set up resources before each test
    void SetUp() override
    {
        // Initialize the file
        fp = fopen(fileManager.getTemporaryFilePath("output.tmp").string().c_str(), "w+");
    }

    // Clean up resources after each test
    void TearDown() override { fclose(fp); }

public:
    // Utility function to run the test, take a builder and strategy, and verify output
    void runTest(ReprFormatter*     strategy,
                 ObjectRepr*        builder,
                 const std::string& expectedOutputPath,
                 const std::string& prefix = "")
    {
        // Build the text dump
        builder->build(strategy);
        strategy->close_section();

        // Flush and seek to the beginning of the file
        fflush(fp);
        fseek(fp, 0, SEEK_SET);

        // Read the file content into a string
        std::ostringstream buffer;
        char               readBuffer[256];
        while (fgets(readBuffer, sizeof(readBuffer), fp) != nullptr)
        {
            buffer << readBuffer;
        }

        // Store the file content in a string
        std::string output = buffer.str();

        // // Open the expected output file
        std::string   filename = fileManager.getInputFilePath(expectedOutputPath).string();
        std::ifstream file(filename);
        if (!file.is_open())
        {
            throw std::runtime_error("Could not open file: " + filename);
        }

        // Read the expected output into a string
        buffer.str("");
        buffer << prefix << file.rdbuf();

        // Verify the output
        EXPECT_EQ(output, buffer.str());
    }
};

class TextRepresentationTest : public RepresentationTest
{
protected:
    TextFormatter* textStrategy;

    void SetUp() override
    {
        RepresentationTest::SetUp();
        textStrategy = new TextFormatter(fp);
    }

    void TearDown() override
    {
        delete textStrategy;
        RepresentationTest::TearDown();
    }

public:
    void runTest(ObjectRepr* builder, const std::string& expectedOutput)
    {
        RepresentationTest::runTest(textStrategy, builder, expectedOutput, "\n");
    }
};

class JsonRepresentationTest : public RepresentationTest
{
protected:
    JsonFormatter* jsonStrategy;

    void SetUp() override
    {
        RepresentationTest::SetUp();
        jsonStrategy = new JsonFormatter(fp);
    }

    void TearDown() override
    {
        delete jsonStrategy;
        RepresentationTest::TearDown();
    }

public:
    void runTest(ObjectRepr* builder, const std::string& expectedOutput)
    {
        RepresentationTest::runTest(jsonStrategy, builder, expectedOutput);
    }
};

class YamlRepresentationTest : public RepresentationTest
{
protected:
    YamlFormatter* yamlStrategy;

    void SetUp() override
    {
        RepresentationTest::SetUp();
        yamlStrategy = new YamlFormatter(fp);
    }

    void TearDown() override
    {
        delete yamlStrategy;
        RepresentationTest::TearDown();
    }

public:
    void runTest(ObjectRepr* builder, const std::string& expectedOutput)
    {
        RepresentationTest::runTest(yamlStrategy, builder, expectedOutput);
    }
};

} // namespace test

} // namespace gmx
