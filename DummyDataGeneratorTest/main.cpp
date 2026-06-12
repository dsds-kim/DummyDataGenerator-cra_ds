#include "Test/DataGeneratorTest.h"
#include "Test/DbConnectionTest.h"
#include "Test/TestRunner.h"
#include <iostream>

int main()
{
    int failures = 0;

    std::cout << "=== DB Connection Tests ===\n\n";
    {
        TestRunner runner;
        registerDbConnectionTests(runner);
        failures += runner.run();
    }

    std::cout << "\n=== Data Generator Tests ===\n\n";
    {
        TestRunner runner;
        registerDataGeneratorTests(runner);
        failures += runner.run();
    }

    return failures > 0 ? 1 : 0;
}
