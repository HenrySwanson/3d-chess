#ifndef UNIT_TEST_INTERNAL_H
#define UNIT_TEST_INTERNAL_H

#include <list>
#include <string>

#include "unit_test.h"

struct TestSuite
{
    std::string name;
    std::list<UnitTest> tests;
    void (*setup_func)(void);
    void (*teardown_func)(void);
};

class MissionControl
{
  public:
    static std::list<TestSuite>& getSuites()
    {
        static std::list<TestSuite> suites;
        return suites;
    }
  private:
    // TODO add error total
};

UnitTest create_and_register(std::string suite_name, std::string test_name,
        TestFunctionPtr test_func)
{
    UnitTest ut;
    ut.name = test_name;
    ut.func = test_func;

    std::list<TestSuite>& suites = MissionControl::getSuites();
    std::list<TestSuite>::iterator it;
    for(it = suites.begin(); it != suites.end(); it++)
    {
        if(it->name == suite_name)
        {
            it->tests.push_back(ut);
            return ut;
        }
    }

    TestSuite ts;
    ts.name = suite_name;
    ts.setup_func = ts.teardown_func = NULL;
    ts.tests.push_back(ut);

    suites.push_front(ts);

    return ut;
}

#endif
