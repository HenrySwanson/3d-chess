#ifndef UNIT_TEST_H
#define UNIT_TEST_H

#include <cstring>
#include <fstream>
#include <list>

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
            bool expected);

    template<typename T, typename U>
    static void logCmp(const char* file, int line, const char* expr_a,
            const char* expr_b, const char* cmp_op, T val_a, U val_b);


    static void logStrCmp(const char* file, int line, const char* expr_a,
            const char* expr_b, const char* cmp_op, const char* val_a,
            const char* val_b);

    // TODO log float/double comparisons with tolerance

  private:
    static bool current_test_failed_;
    static int num_tests_completed_;
    static int num_tests_failed_;

    static std::ofstream logfile_;
};

template<typename T, typename U>
void MissionControl::logCmp(const char* file, int line, const char* expr_a,
        const char* expr_b, const char* op, T val_a, U val_b)
{
    current_test_failed_ = true;

    logfile_ << "Failed comparision: " << file << ":" << line << ": " <<
            expr_a << " " << op << " " << expr_b << std::endl;
    logfile_ << "  Values were: \"" << val_a << "\" and \"" << val_b << "\"" <<
            std::endl;
}

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

#define IF_NOT(b)                                                             \
    if(b){;}else                                                              \


//====Simple Boolean Macros====

#define ASSERT_TRUE(b)                                                        \
    IF_NOT( b )                                                               \
    {                                                                         \
        MissionControl::logBool(__FILE__, __LINE__, #b, true);                \
        return;                                                               \
    }

#define EXPECT_TRUE(b)                                                        \
    IF_NOT( b )                                                               \
    {                                                                         \
        MissionControl::logBool(__FILE__, __LINE__, #b, true);                \
    }

#define ASSERT_FALSE(b)                                                       \
    IF_NOT( !b )                                                              \
    {                                                                         \
        MissionControl::logBool(__FILE__, __LINE__, #b, false);               \
        return;                                                               \
    }

#define EXPECT_FALSE(b)                                                       \
    IF_NOT( !b )                                                              \
    {                                                                         \
        MissionControl::logBool(__FILE__, __LINE__, #b, false);               \
    }

//====Comparison Macros====

#define ASSERT_CMP(a, b, op)                                                  \
    IF_NOT( (a) op (b) )                                                      \
    {                                                                         \
        MissionControl::logCmp(__FILE__, __LINE__, #a, #b, #op, (a), (b));    \
        return;                                                               \
    }

#define EXPECT_CMP(a, b, op)                                                  \
    IF_NOT( (a) op (b) )                                                      \
    {                                                                         \
        MissionControl::logCmp(__FILE__, __LINE__, #a, #b, #op, (a), (b));    \
    }

#define ASSERT_EQ(a, b) ASSERT_CMP(a, b, ==)
#define ASSERT_NE(a, b) ASSERT_CMP(a, b, !=)
#define ASSERT_LT(a, b) ASSERT_CMP(a, b,  <)
#define ASSERT_GT(a, b) ASSERT_CMP(a, b,  >)
#define ASSERT_LE(a, b) ASSERT_CMP(a, b, <=)
#define ASSERT_GE(a, b) ASSERT_CMP(a, b, >=)

#define EXPECT_EQ(a, b) EXPECT_CMP(a, b, ==)
#define EXPECT_NE(a, b) EXPECT_CMP(a, b, !=)
#define EXPECT_LT(a, b) EXPECT_CMP(a, b,  <)
#define EXPECT_GT(a, b) EXPECT_CMP(a, b,  >)
#define EXPECT_LE(a, b) EXPECT_CMP(a, b, <=)
#define EXPECT_GE(a, b) EXPECT_CMP(a, b, >=)

//====String Comparision Macros====

#define ASSERT_STR_CMP(a, b, op)                                              \
    IF_NOT( strcmp((a), (b)) op 0 )                                           \
    {                                                                         \
        MissionControl::logStrCmp(__FILE__, __LINE__, #a, #b, #op, (a), (b)); \                                                         \
        return;                                                               \
    }

#define EXPECT_STR_CMP(a, b, op)                                              \
    IF_NOT( strcmp((a), (b)) op 0 )                                           \
    {                                                                         \
        MissionControl::logStrCmp(__FILE__, __LINE__, #a, #b, #op, (a), (b)); \
    }

#define ASSERT_STR_EQ(a, b) ASSERT_STR_CMP(a, b, ==)
#define ASSERT_STR_NE(a, b) ASSERT_STR_CMP(a, b, !=)
#define ASSERT_STR_LT(a, b) ASSERT_STR_CMP(a, b,  <)
#define ASSERT_STR_GT(a, b) ASSERT_STR_CMP(a, b,  >)
#define ASSERT_STR_LE(a, b) ASSERT_STR_CMP(a, b, <=)
#define ASSERT_STR_GE(a, b) ASSERT_STR_CMP(a, b, >=)

#define EXPECT_STR_EQ(a, b) EXPECT_STR_CMP(a, b, ==)
#define EXPECT_STR_NE(a, b) EXPECT_STR_CMP(a, b, !=)
#define EXPECT_STR_LT(a, b) EXPECT_STR_CMP(a, b,  <)
#define EXPECT_STR_GT(a, b) EXPECT_STR_CMP(a, b,  >)
#define EXPECT_STR_LE(a, b) EXPECT_STR_CMP(a, b, <=)
#define EXPECT_STR_GE(a, b) EXPECT_STR_CMP(a, b, >=)

#endif
