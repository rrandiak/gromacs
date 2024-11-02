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

class PrintFunctionTest: public ::testing::Test
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
    void runTest(const std::function<void(FILE*)>& func, const std::string& expectedOutput)
    {
        // Execute the function
        func(fp);

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

} // namespace test

} // namespace gmx
