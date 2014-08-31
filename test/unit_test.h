#ifndef UNIT_TEST_H
#define UNIT_TEST_H

#include <cstring>
#include <fstream>
#include <list>

/** Since all tests should be independent, this is their type signature. */
typedef void (*TestFunctionPtr)(void);

/** Bundles together the name and implementation of a unit test. */
struct UnitTest
{
    /** The name of the test. */
    const char* name;

    /** A pointer to the test function. */
    TestFunctionPtr func;
};

/**
 * Bundles together many unit tests, and gives that collection a name. Also,
 * has the possibility to include setup and teardown functions.
 */
struct TestSuite
{
    /** The name of the suite. */
    const char* name;

    /** The list of unit tests in the suite. */
    std::list<UnitTest> tests;

    /** If non-null, is run before all tests in the suite. */
    void (*setup_func)(void);

    /** If non-null, is run after all tests in the suite. */
    void (*teardown_func)(void);
};

/** A singleton/static class that manages all unit tests and their logs. */
class MissionControl
{
  public:
    /**
     * When we declare a test, we call MissionControl::createAndRegister, which
     * creates the test and adds it to a list. But because the order in which
     * global variables are instantiated is ambiguous, we have to be more
     * clever in how we construct this is. When this function is called, it
     * automatically constructs the list "suites", which persists forever in
     * this function. A bit strange, but it seems to be common for singleton
     * classes that have to touch global state.
     */
    static std::list<TestSuite>& suites()
    {
        static std::list<TestSuite> suites;
        return suites;
    }

    /**
     * Constructs a unit test and adds it to the global list of suites.
     * Then it returns the test, because declaring tests is done by
     * instantiating a global variable, which must receive some value in order
     * to properly compile.
     *
     * Parameters:
     *     suite_name - the name of the test suite
     *     test_name - the name of the unit test
     *     test_function - a pointer to the test function
     */
    static UnitTest createAndRegister(const char* suite_name,
            const char* test_name, TestFunctionPtr test_func);

    /** Activates logging and runs all tests registered. */
    static void runAll();

    /** Runs the given test, assuming logging is active. */
    static void runTest(const UnitTest& ut);

    /** Logs the result of {ASSERT/EXPECT}_{TRUE/FALSE}. */
    static void logBool(const char* file, int line, const char* expr,
            bool expected);

    // TODO i don't like having this templated, because it forces me to put
    // the definition into the header, so it gets everywhere. But is there an
    // alternative?

    /** Logs the result of {ASSERT/EXPECT}_CMP. */
    template<typename T, typename U>
    static void logCmp(const char* file, int line, const char* expr_a,
            const char* expr_b, const char* op, T val_a, U val_b);


    /** Logs the result of {ASSERT/EXPECT}_STR_CMP. */
    static void logStrCmp(const char* file, int line, const char* expr_a,
            const char* expr_b, const char* op, const char* val_a,
            const char* val_b);

    // TODO log float/double comparisons with tolerance

  private:
    /** Whether the currently running test has failed. */
    static bool current_test_failed_;

    /** How many tests have been run so far. */
    static int num_tests_completed_;

    /** How many tests have failed so far. */
    static int num_tests_failed_;

    /** The file to write to for logging purposes. */
    static std::ofstream logfile_;
};

// ---- IMPLEMENTATION ----
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


/** Creates a (hopefully) unique variable name to store the unit test in. */
#define _TEST_NAME(suite, name)                                               \
    unit_test_##suite##_##name

/** Creates a (hopefully) unique function name to declare the test function. */
#define _TEST_FUNC_NAME(suite, name)                                          \
    unit_test_func_##suite##_##name

/**
 * When called, this macro creates a unit test, assigns it to a global
 * variable, then registers it with MissionControl. The last line is the method
 * signature, so this should be followed by a brace, then the function
 * definition, and then a closed brace, as if this were an actual method
 * signature.
 */
#define TEST(suite, name)\
    void _TEST_FUNC_NAME(suite, name)(void);                                  \
    UnitTest _TEST_NAME(suite, name) =                                        \
            MissionControl::createAndRegister(#suite, #name,                  \
            &(_TEST_FUNC_NAME(suite, name)));                                 \
    void _TEST_FUNC_NAME(suite, name)(void)

// TODO add setup/teardown function registration

/** This is used as an alternative to filling the page with !s and ()s. */
#define _IF_NOT(b)                                                            \
    if(b){;}else                                                              \

//=======================//
// Simple Boolean Macros //
//=======================//

/** Asserts that the given condition is true. */
#define ASSERT_TRUE(b)                                                        \
    _IF_NOT( (b) )                                                            \
    {                                                                         \
        MissionControl::logBool(__FILE__, __LINE__, #b, true);                \
        return;                                                               \
    }

/** Expects that the given condition is true. */
#define EXPECT_TRUE(b)                                                        \
    _IF_NOT( (b) )                                                            \
    {                                                                         \
        MissionControl::logBool(__FILE__, __LINE__, #b, true);                \
    }

/** Asserts that the given condition is false. */
#define ASSERT_FALSE(b)                                                       \
    _IF_NOT( !(b) )                                                           \
    {                                                                         \
        MissionControl::logBool(__FILE__, __LINE__, #b, false);               \
        return;                                                               \
    }

/** Expects that the given condition is false. */
#define EXPECT_FALSE(b)                                                       \
    _IF_NOT( !(b) )                                                           \
    {                                                                         \
        MissionControl::logBool(__FILE__, __LINE__, #b, false);               \
    }

//===================//
// Comparison Macros //
//===================//

/** Asserts that a op b, where op is a comparison operator. */
#define ASSERT_CMP(a, b, op)                                                  \
    _IF_NOT( (a) op (b) )                                                     \
    {                                                                         \
        MissionControl::logCmp(__FILE__, __LINE__, #a, #b, #op, (a), (b));    \
        return;                                                               \
    }

/** Expects that a op b, where op is a comparison operator. */
#define EXPECT_CMP(a, b, op)                                                  \
    _IF_NOT( (a) op (b) )                                                     \
    {                                                                         \
        MissionControl::logCmp(__FILE__, __LINE__, #a, #b, #op, (a), (b));    \
    }

/*
 * Specializations of {ASSERT/EXPECT}_CMP. Will only work for types that
 * implement the comparison operator, and the << stream operator (for logging).
 */
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

//==========================//
// String Comparison Macros //
//==========================//

/**
 * Asserts that a op b, where op is a comparison operator. The order used is
 * the order given by strcmp.
 */
#define ASSERT_STR_CMP(a, b, op)                                              \
    _IF_NOT( strcmp((a), (b)) op 0 )                                          \
    {                                                                         \
        MissionControl::logStrCmp(__FILE__, __LINE__, #a, #b, #op, (a), (b)); \
        return;                                                               \
    }

/**
 * Expects that a op b, where op is a comparison operator. The order used is
 * the order given by strcmp.
 */
#define EXPECT_STR_CMP(a, b, op)                                              \
    _IF_NOT( strcmp((a), (b)) op 0 )                                          \
    {                                                                         \
        MissionControl::logStrCmp(__FILE__, __LINE__, #a, #b, #op, (a), (b)); \
    }

/* Specializations of {ASSERT/EXPECT}_STR_CMP. */
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
