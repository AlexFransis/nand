#include <iostream>
#include <assert.h>
#include "tokenizer.h"


bool tokenizer_test()
{
        Tokenizer t;
        std::cout << "[INFO] Running tests for tokenizer...\n";
        // arrange
        std::string input1 = "int main() // this is a comment";
        std::string expected1 = "int main() ";

        // act
        std::string actual1 = t.trim_comments(input1);

        // assert
        bool result = expected1 == actual1;
        if (!result)
        {
                std::cout << "[WARN] Test failed for tokenizer method <trim_comments>" << std::endl;
                std::cout << "[WARN] expected: " << expected1 << std::endl;
                std::cout << "[WARN] actual: " << actual1 << std::endl;
        }
        return result;
}


int main(int argc, char **argv)
{
        bool passed = true;
        passed = passed && tokenizer_test();
        if (passed) {
                std::cout << "[INFO] All tests passed successfully...\n";
        }
        std::cout << "[INFO] Testing completed...";
        return 0;
}
