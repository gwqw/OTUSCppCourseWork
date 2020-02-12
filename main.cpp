#include <iostream>
#include <memory>
#include <stdexcept>

#include "task_generator.h"
#include "response_calculator.h"
#include "result_saver.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "course_work -- multithreaded task manager\n"
                "course_work <tasks_number> [<complexity>]\n"
                "tasks_number -- 1024, 2048, 4096, 8192, 16384\n"
                "comlexity -- multiplier for every task: 10 - 50, default = 10" << endl;
        return 0;
    }

    try {
        size_t tasks_number = stol(argv[1]);
        if (tasks_number > 60'000) {
            throw invalid_argument("tasks number is too big");
        }
        int task_complexity = 10;
        if (argc > 2) {
            task_complexity = stol(argv[2]);
        }
        auto task_generator = make_unique<RandomTaskGenerator>(TaskInput{tasks_number, task_complexity});
        ResponseCalculator resp_calculator(move(task_generator));
        //createAndSubscribe<PercentLogger>(resp_calculator, tasks_number, cout);
        createAndSubscribe<ResultSaver>(resp_calculator, "test.mtx");
        resp_calculator.run();
    } catch (const exception& e) {
        cerr << e.what() << endl;
    }
    return 0;
}
