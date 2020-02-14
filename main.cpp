#include <iostream>
#include <memory>
#include <stdexcept>
#include <thread>

#include "task_generator.h"
#include "calc_task_mgr.h"
#include "result_saver.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "course_work -- multithreaded task manager\n"
                "course_work <tasks_number> [<threads_count> <complexity>]\n"
                "threads_count -- number of threads (default = 1)\n"
                "tasks_number -- 1024, 2048, 4096, 8192, 16384\n"
                "complexity -- multiplier for every task: 30 - 150, default = 30" << endl;
        return 0;
    }

    try {
        size_t tasks_number = stol(argv[1]);
        if (tasks_number > 60'000) {
            throw invalid_argument("tasks number is too big");
        }
        int threads_num = 1;//max(thread::hardware_concurrency(), 1u);
        int task_complexity = 30;
        if (argc > 2) {
            threads_num = stol(argv[2]);
            if (argc > 3) {
                task_complexity = stol(argv[3]);
            }
        }

        auto task_generator = make_unique<SimpleTaskCalculator>(
                TaskInput{tasks_number, task_complexity}
        );
        CalcTaskMgr calc_task_mgr(move(task_generator), threads_num);
        createAndSubscribe<ResultSaver>(calc_task_mgr, tasks_number, "test.mtx");
        //createAndSubscribe<PercentLogger>(calc_task_mgr, tasks_number, cout);
        calc_task_mgr.run();
    } catch (const exception& e) {
        cerr << e.what() << endl;
    }
    return 0;
}
