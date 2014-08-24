#include "unit_test.h"
#include "unit_test_internal.h"

#include <iostream>
#include <list>

using std::list;
using std::cout;
using std::endl;

int main(void)
{
    cout << "Beginning tests..." << endl;

    list<TestSuite>& suites = MissionControl::getSuites();
    list<TestSuite>::iterator it;
    for(it = suites.begin(); it != suites.end(); it++)
    {
        cout << "Suite: " << it->name << endl;
        list<UnitTest>& tests = it->tests;
        list<UnitTest>::iterator jt;
        for(jt = tests.begin(); jt != tests.end(); jt++)
        {
            cout << "Test: " << jt->name << endl;
            (*(jt->func))();
        }
    }

    cout << "End of tests" << endl;
}
