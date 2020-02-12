#include "result_saver.h"

using namespace std;

void ResultSaver::update(ResultHolder calc_result) {
    file_.write(reinterpret_cast<char*>(calc_result->line.data()),
                calc_result->line.size() * sizeof(double));
}

void StreamLogger::update(ResultHolder calc_result) {
    out_ << calc_result->task_num << endl;
}

void StreamSaver::update(ResultHolder calc_result) {
    out_ << calc_result->task_num << ": ";
    const auto& line = calc_result->line;
    for (auto value : line) {
        out_ << value << " ";
    }
    out_ << endl;
}

void PercentLogger::update(ResultHolder calc_result) {
    int percent = 100 * calc_result->task_num / tasks_size_;
    if (percent > cur_percent_) {
        cur_percent_ = percent;
        out_ << cur_percent_ << '%' << endl;
    }
}
