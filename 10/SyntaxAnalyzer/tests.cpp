#include <iostream>
#include <assert.h>
#include "tokenizer.h"


int tokenizer_is_symbol_test()
{
        Tokenizer t;
        std::string input1 = "{";
        bool expected1 = true;
        bool actual1 = t.is_symbol(input1);

        std::string input2 = "=";
        bool expected2 = true;
        bool actual2 = t.is_symbol(input2);

        std::string input3 = "A";
        bool expected3 = false;
        bool actual3 = t.is_symbol(input3);

        if (expected1 == actual1 && expected2 == actual2 && expected3 == actual3) {
                std::cout << "[INFO] Testing tokenizer::is_symbol... PASSED\n";
                return 0;
        } else {
                std::cout << "[INFO] Testing tokenizer::is_symbol... FAILED\n";
                return 1;
        }
}

int tokenizer_is_string_test()
{
        Tokenizer t;
        std::string input1 = "\"This is a string test\"";
        bool expected1 = true;
        bool actual1 = t.is_string(input1);
        bool result1 = expected1 == actual1;

        std::string input2 = "\" another string test. This time containing punctuation!\"";
        bool expected2 = true;
        bool actual2 = t.is_string(input2);
        bool result2 = expected2 == actual2;

        std::string input3 = "\"A string with a newline\n\"";
        bool expected3 = false;
        bool actual3 = t.is_string(input3);
        bool result3 = expected3 == actual3;

        std::string input4 = "\"a missing double quote";
        bool expected4 = false;
        bool actual4 = t.is_string(input4);
        bool result4 = expected4 == actual4;

        if (result1 && result2 && result3 && result4) {
                std::cout << "[INFO] Testing tokenizer::is_string... PASSED\n";
                return 0;
        } else {
                std::cout << "[INFO] Testing tokenizer::is_string... FAILED\n";
                return 1;
        }
}

int tokenizer_is_identifier_test()
{
        Tokenizer t;
        std::string input1 = "validIdentifier";
        bool expected1 = true;
        bool actual1 = t.is_identifier(input1);
        bool result1 = expected1 == actual1;

        std::string input2 = "__validIdentifier";
        bool expected2 = true;
        bool actual2 = t.is_identifier(input2);
        bool result2 = expected2 == actual2;

        std::string input3 = "00invalidIdentifier";
        bool expected3 = false;
        bool actual3 = t.is_identifier(input3);
        bool result3 = expected3 == actual3;

        std::string input4 = "invlidIdentifier!";
        bool expected4 = false;
        bool actual4 = t.is_identifier(input4);
        bool result4 = expected4 == actual4;

        std::string input5 = "class";
        bool expected5 = false;
        bool actual5 = t.is_identifier(input5);
        bool result5 = expected5 == actual5;

        if (result1 && result2 && result3 && result4 && result5) {
                std::cout << "[INFO] Testing tokenizer::is_identifier... PASSED\n";
                return 0;
        } else {
                std::cout << "[INFO] Testing tokenizer::is_identifier... FAILED\n";
                return 1;
        }

}

int tokenizer_is_ws_or_comment_test()
{
        Tokenizer t;
        std::string input1 = "code // comment here";
        bool expected1 = false;
        bool actual1 = t.is_ws_or_comment(input1);
        bool result1 = expected1 == actual1;
}

int tokenizer_test()
{
        int failed = 0;
        std::cout << "[INFO] Running tests for tokenizer...\n";

        failed += tokenizer_is_identifier_test();
        failed += tokenizer_is_string_test();
        failed += tokenizer_is_symbol_test();

        return failed;
}


int main(int argc, char **argv)
{
        int failed = 0;
        failed += tokenizer_test();
        std::cout << "[INFO] Testing completed...\n";
        std::cout << "[INFO] " << failed << " failed tests...\n";
        return 0;
}
