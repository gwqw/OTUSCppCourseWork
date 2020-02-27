#define BOOST_TEST_MODULE coursework_test_module
#include <boost/test/unit_test.hpp>

#include <sstream>

#include "task_generator.h"
#include "calc_task_mgr.h"
#include "result_saver.h"

using namespace std;

BOOST_AUTO_TEST_SUITE(coursework_test_suite)

	BOOST_AUTO_TEST_CASE(test_) {
        constexpr int TASK_NUM  = 5;
        constexpr int TASK_COMPLEX = 0; // unused
        stringstream out;
        {
            auto task_generator = make_unique<TestTaskCalculator>(
                    TaskInput{TASK_NUM, TASK_COMPLEX}
            );
            CalcTaskMgr calc_task_mgr(move(task_generator), 2);
            createAndSubscribe<StreamSaver>(calc_task_mgr, TASK_NUM, out);
            //createAndSubscribe<PercentLogger>(calc_task_mgr, tasks_number, cout);
            calc_task_mgr.run();
        }
        string expected {
            "0: 0 0 0 0 0 \n"
               "1: 0 1 0 0 0 \n"
               "2: 0 1 2 0 0 \n"
               "3: 0 1 2 3 0 \n"
               "4: 0 1 2 3 4 \n"
        };
        BOOST_TEST(out.str() == expected);
	}

BOOST_AUTO_TEST_SUITE_END()
