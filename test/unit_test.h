#ifndef UNIT_TEST_H
#define UNIT_TEST_H

#include <string>

// TODO Document this clusterfuck, especially that bs going on in TEST

typedef void (*TestFunctionPtr)(void);

struct UnitTest
{
    std::string name;
    TestFunctionPtr func;
};

UnitTest create_and_register(std::string suite_name, std::string test_name,
        TestFunctionPtr test_func);

#define _TEST_NAME(suite, name)\
    unit_test_##suite##_##name

#define _TEST_FUNC_NAME(suite, name)\
    unit_test_func_##suite##_##name

#define TEST(suite, name)\
    void _TEST_FUNC_NAME(suite, name)(void);\
    UnitTest _TEST_NAME(suite, name) = create_and_register(#suite, #name,\
            &(_TEST_FUNC_NAME(suite, name)));\
    void _TEST_FUNC_NAME(suite, name)(void)

// TODO add setup/teardown function registration

#endif
