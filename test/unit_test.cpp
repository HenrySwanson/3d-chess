#include "unit_test.h"
//#include "unit_test_internal.h"

#include <iostream>
#include <list>
#include <cstring>

using std::list;
using std::cout;
using std::endl;

UnitTest MissionControl::createAndRegister(const char* suite_name,
        const char* test_name, TestFunctionPtr test_func)
{
    UnitTest ut;
    ut.name = test_name;
    ut.func = test_func;

    std::list<TestSuite>& ss = suites();
    std::list<TestSuite>::iterator it;
    for(it = ss.begin(); it != ss.end(); it++)
    {
        if(strcmp(it->name, suite_name) == 0)
        {
            it->tests.push_back(ut);
            return ut;
        }
    }

    TestSuite ts;
    ts.name = suite_name;
    ts.setup_func = ts.teardown_func = NULL;
    ts.tests.push_back(ut);

    ss.push_front(ts);

    return ut;
}

void MissionControl::runAll()
{
    // TODO what's convention?
    logfile_.open("unit_tests.log");
    logfile_ << std::boolalpha;

    cout << "Beginning tests..." << endl;

    list<TestSuite>& ss = suites();
    list<TestSuite>::iterator it;
    for(it = ss.begin(); it != ss.end(); it++)
    {
        cout << "  Starting suite: " << it->name << endl;
        list<UnitTest>& tests = it->tests;
        list<UnitTest>::iterator jt;
        for(jt = tests.begin(); jt != tests.end(); jt++)
            runTest(*jt);
    }

    cout << "End of tests: " << num_tests_completed_ << " completed, "
            << num_tests_failed_ << " failed." << endl;

    logfile_.close();
}

void MissionControl::runTest(UnitTest ut)
{
    current_test_failed_ = false;

    cout << "    Test: " << ut.name;
    logfile_ << "Test: " << ut.name << endl;

    (*(ut.func))();

    if(current_test_failed_)
    {
        num_tests_failed_++;
        cout << "......failed!" << endl;
    }
    else
    {
        cout << "......succeeded." << endl;
    }

    num_tests_completed_++;
}

void MissionControl::logBool(const char* file, int line, const char* expr,
        bool expected, bool fatal)
{
    current_test_failed_ = true;
    const char* noun = fatal ? "assertion" : "expectation";
    logfile_ << "Failed " << noun << ": " << file << ":" << line << ": " <<
            expr << " should have been " << expected << "." << endl;
}

bool MissionControl::current_test_failed_ = false;
int MissionControl::num_tests_completed_ = 0;
int MissionControl::num_tests_failed_ = 0;
std::ofstream MissionControl::logfile_;

int main(void)
{
    MissionControl::runAll();
}
