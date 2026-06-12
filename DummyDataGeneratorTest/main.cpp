#include "Test/DbConnectionTest.h"
#include "Test/TestRunner.h"
#include <iostream>

int main()
{
    std::cout << "=== DB Connection Tests ===\n\n";

    TestRunner runner;
    registerDbConnectionTests(runner);

    int failures = runner.run();
    return failures > 0 ? 1 : 0;
}
