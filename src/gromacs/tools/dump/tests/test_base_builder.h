#include <gtest/gtest.h>

#include "testutils/testfilemanager.h"

#include "gromacs/tools/dump/dump_builder.h"
#include "gromacs/tools/dump/dump_strategy.h"
#include "gromacs/tools/dump/strategies/json_strategy.h"
#include "gromacs/tools/dump/strategies/text_strategy.h"
#include "gromacs/tools/dump/strategies/yaml_strategy.h"

namespace gmx
{

namespace test
{

class DumpBuilderTest : public ::testing::Test
{
protected:
    FILE* fp;

    // Set up resources before each test
    void SetUp() override
    {
        // Initialize the file
        TestFileManager fileManager;
        fp = fopen(
            fileManager.getTemporaryFilePath("output.tmp").string().c_str(),
            "w+"
        );
    }

    // Clean up resources after each test
    void TearDown() override
    {
        fclose(fp);
    }

public:
    // Utility function to run the test, take a builder and strategy, and verify output
    void runTest(DumpStrategy* strategy, DumpBuilder* builder, const std::string& expectedOutput)
    {
        // Build the text dump
        builder->build(strategy);
        strategy->close_section();

        // Flush and seek to the beginning of the file
        fflush(fp);
        fseek(fp, 0, SEEK_SET);

        // Read the file content into a string
        std::ostringstream buffer;
        char readBuffer[256];
        while (fgets(readBuffer, sizeof(readBuffer), fp) != nullptr)
        {
            buffer << readBuffer;
        }

        // Store the file content in a string
        std::string output = buffer.str();

        // Verify the output
        EXPECT_EQ(output, expectedOutput);
    }
};

class DumpBuilderTextTest : public DumpBuilderTest
{
protected:
    TextStrategy* textStrategy;

    void SetUp() override
    {
        DumpBuilderTest::SetUp();
        textStrategy = new TextStrategy(fp);
    }

    void TearDown() override
    {
        delete textStrategy;
        DumpBuilderTest::TearDown();
    }

public:
    void runTest(DumpBuilder* builder, const std::string& expectedOutput)
    {
        DumpBuilderTest::runTest(textStrategy, builder, expectedOutput);
    }
};

class DumpBuilderJsonTest : public DumpBuilderTest
{
protected:
    JsonStrategy* jsonStrategy;

    void SetUp() override
    {
        DumpBuilderTest::SetUp();
        jsonStrategy = new JsonStrategy(fp);
    }

    void TearDown() override
    {
        delete jsonStrategy;
        DumpBuilderTest::TearDown();
    }

public:
    void runTest(DumpBuilder* builder, const std::string& expectedOutput)
    {
        DumpBuilderTest::runTest(jsonStrategy, builder, expectedOutput);
    }
};

class DumpBuilderYamlTest : public DumpBuilderTest
{
protected:
    YamlStrategy* yamlStrategy;

    void SetUp() override
    {
        DumpBuilderTest::SetUp();
        yamlStrategy = new YamlStrategy(fp);
    }

    void TearDown() override
    {
        delete yamlStrategy;
        DumpBuilderTest::TearDown();
    }

public:
    void runTest(DumpBuilder* builder, const std::string& expectedOutput)
    {
        DumpBuilderTest::runTest(yamlStrategy, builder, expectedOutput);
    }
};

} // namespace test

} // namespace gmx
