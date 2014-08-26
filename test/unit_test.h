#ifndef UNIT_TEST_H
#define UNIT_TEST_H

#include <fstream>
#include <list>
#include <string>

// TODO Document this clusterfuck, especially that bs going on in TEST

typedef void (*TestFunctionPtr)(void);

struct UnitTest
{
    const char* name;
    TestFunctionPtr func;
};

struct TestSuite
{
    const char* name;
    std::list<UnitTest> tests;
    void (*setup_func)(void);
    void (*teardown_func)(void);
};

class MissionControl
{
  public: // TODO document why you did this (static initialization order fiasco)
    static std::list<TestSuite>& suites()
    {
        static std::list<TestSuite> suites;
        return suites;
    }

    static UnitTest createAndRegister(const char* suite_name,
            const char* test_name, TestFunctionPtr test_func);

    static void runAll();
    static void runTest(UnitTest ut);

    static void logBool(const char* file, int line, const char* expr,
            bool expected, bool fatal);

    // TODO log str comparison

    // TODO log comparison (<, ==, !=, <=)
    //     [to get types right, use templates. Foo<var> is Foo<Type>]
    //     [or provide type in macro, if you want C compatibility.]

    // TODO log float/double comparisons with tolerance

  private:
    static bool current_test_failed_;
    static int num_tests_completed_;
    static int num_tests_failed_;

    static std::ofstream logfile_;
};

#define _TEST_NAME(suite, name)                                               \
    unit_test_##suite##_##name

#define _TEST_FUNC_NAME(suite, name)                                          \
    unit_test_func_##suite##_##name

#define TEST(suite, name)\
    void _TEST_FUNC_NAME(suite, name)(void);                                  \
    UnitTest _TEST_NAME(suite, name) =                                        \
            MissionControl::createAndRegister(#suite, #name,                  \
            &(_TEST_FUNC_NAME(suite, name)));                                 \
    void _TEST_FUNC_NAME(suite, name)(void)

// TODO add setup/teardown function registration

#define ASSERT_TRUE(b)                                                        \
    if(!(b))                                                                  \
    {                                                                         \
        MissionControl::logBool(__FILE__, __LINE__, #b, true, true);          \
        return;                                                               \
    }

#define EXPECT_TRUE(b)                                                        \
    if(!(b))                                                                  \
    {                                                                         \
        MissionControl::logBool(__FILE__, __LINE__, #b, true, false);         \
    }

#define ASSERT_FALSE(b)                                                       \
    if(b)                                                                     \
    {                                                                         \
        MissionControl::logBool(__FILE__, __LINE__, #b, false, true);         \
        return;                                                               \
    }

#define EXPECT_FALSE(b)                                                       \
    if(b)                                                                     \
    {                                                                         \
        MissionControl::logBool(__FILE__, __LINE__, #b, false, false);        \
    }

#endif
