#include "result_saver.h"

using namespace std;

ResultSaver::ResultSaver(std::size_t tasks_size, const std::string &filename)
    : tasks_size_(tasks_size), file_(filename, std::ios_base::binary)
#ifdef MULTITHREAD
        , results(tasks_size_)
#endif
{
#ifdef MULTITHREAD
    auto save_task = [this, first_idx = size_t(0)]() mutable {
        while (first_idx < tasks_size_) {
            std::unique_lock<std::mutex> lk(this->cv_m_);
            condition_.wait(lk, [this, first_idx](){
                return this->results[first_idx] != nullptr;
            });
            lk.unlock();
            size_t last_idx = first_idx;
            for (; last_idx < tasks_size_; ++last_idx) {
                if (results[last_idx] == nullptr) break;
            }
            for (size_t i = first_idx; i < last_idx; ++i) {
                auto calc_res = move(results[i]);
                file_.write(reinterpret_cast<char *>(calc_res->line.data()),
                            calc_res->line.size() * sizeof(double));
            }
            first_idx = last_idx;
        }
    };
    thread_ = thread(save_task);
#endif
}

void ResultSaver::update(ResultHolder calc_result) {
#ifdef MULTITHREAD
    // add calculated line to results
    results[calc_result->task_num] = move(calc_result);
    condition_.notify_one();
#else
    file_.write(reinterpret_cast<char*>(calc_result->line.data()),
                calc_result->line.size() * sizeof(double));
#endif
}

ResultSaver::~ResultSaver() {
#ifdef MULTITHREAD
    thread_.join();
#endif
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
    int percent = int(100 * calc_result->task_num / tasks_size_);
    if (percent > cur_percent_) {
        cur_percent_ = percent;
        out_ << cur_percent_ << '%' << endl;
    }
}
