#include "result_saver.h"

#include <cmath>

using namespace std;

ResultSaver::ResultSaver(std::size_t tasks_size, const std::string &filename)
    : tasks_size_(tasks_size), file_(filename, std::ios_base::binary)
#ifdef MULTITHREAD
        , results(tasks_size_)
#endif
{
#ifdef MULTITHREAD
#ifndef SAVE_SAME_THREAD
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
#endif
}

void ResultSaver::update(ResultHolder calc_result) {
#ifdef MULTITHREAD
    // add calculated line to results
    results[calc_result->task_num] = move(calc_result);
#ifdef SAVE_SAME_THREAD
    saveToFile();
#else
    condition_.notify_one();
#endif
#else
    file_.write(reinterpret_cast<const char*>(calc_result->line.data()),
                calc_result->line.size() * sizeof(double));
#endif
}

#ifdef SAVE_SAME_THREAD
void ResultSaver::saveToFile() {
//    lock_guard<mutex> lk(mtx_);
    unique_lock<mutex> lk(mtx_, defer_lock);
    if (lk.try_lock()) {
        size_t last_idx = first_idx_;
        for (; last_idx < tasks_size_; ++last_idx) {
            if (results[last_idx] == nullptr) break;
        }
        for (size_t i = first_idx_; i < last_idx; ++i) {
            auto calc_res = move(results[i]);
            file_.write(reinterpret_cast<char *>(calc_res->line.data()),
                        calc_res->line.size() * sizeof(double));
        }
        first_idx_ = last_idx;
    }
}
#endif

ResultSaver::~ResultSaver() {
#ifdef MULTITHREAD
#ifdef SAVE_SAME_THREAD
    if (first_idx_ != tasks_size_) {
        saveToFile();
    }
#else
    thread_.join();
#endif
#endif
}

void PercentLogger::update(ResultHolder calc_result) {
    ++calculated_;
    int percent =  (int)lround(double(calculated_) / tasks_size_ * 100);
    if (percent > cur_percent_) {
        cur_percent_ = percent;
        out_ << cur_percent_ << "%, ";
        out_.flush();
    }
}

void StreamLogger::update(ResultHolder calc_result) {
    out_ << calc_result->task_num << endl;
}

StreamSaver::StreamSaver(std::size_t tasks_size, std::ostream &out)
        : tasks_size_(tasks_size), out_(out)
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
                out_ << calc_res->task_num << ": ";
                const auto& line = calc_res->line;
                for (auto value : line) {
                    out_ << value << " ";
                }
                out_ << endl;
            }
            first_idx = last_idx;
        }
    };

    thread_ = thread(save_task);
#endif
}

void StreamSaver::update(ResultHolder calc_result) {
#ifdef MULTITHREAD
    results[calc_result->task_num] = move(calc_result);
    condition_.notify_one();
#else
    out_ << calc_result->task_num << ": ";
    const auto& line = calc_result->line;
    for (auto value : line) {
        out_ << value << " ";
    }
    out_ << endl;
#endif
}

StreamSaver::~StreamSaver() {
#ifdef MULTITHREAD
    thread_.join();
#endif
}



