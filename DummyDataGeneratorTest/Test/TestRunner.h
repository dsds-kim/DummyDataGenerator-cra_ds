#pragma once
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

struct TestCase
{
    std::string           name;
    std::function<void()> fn;
};

class TestRunner
{
public:
    void add(std::string name, std::function<void()> fn)
    {
        m_tests.push_back({ std::move(name), std::move(fn) });
    }

    // Returns number of failed tests.
    int run()
    {
        int passed = 0, failed = 0;
        for (auto& tc : m_tests)
        {
            try
            {
                tc.fn();
                std::cout << "  [PASS] " << tc.name << '\n';
                ++passed;
            }
            catch (const std::exception& e)
            {
                std::cout << "  [FAIL] " << tc.name << " -- " << e.what() << '\n';
                ++failed;
            }
            catch (...)
            {
                std::cout << "  [FAIL] " << tc.name << " -- unknown exception\n";
                ++failed;
            }
        }
        std::cout << '\n' << passed << " passed, " << failed << " failed\n";
        return failed;
    }

private:
    std::vector<TestCase> m_tests;
};

#define ASSERT_TRUE(cond) \
    do { \
        if (!(cond)) \
            throw std::runtime_error("ASSERT_TRUE failed: " #cond); \
    } while (0)

#define ASSERT_THROWS(expr) \
    do { \
        bool caught_ = false; \
        try { expr; } catch (const std::exception&) { caught_ = true; } \
        if (!caught_) \
            throw std::runtime_error("expected exception was not thrown: " #expr); \
    } while (0)
