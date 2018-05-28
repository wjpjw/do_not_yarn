//
// Created by jipeng on 5/26/18.
//

#ifndef DO_NOT_YARN_THREADPOOL_H
#define DO_NOT_YARN_THREADPOOL_H

#include <mutex>
#include <condition_variable>
#include <functional>
#include <queue>
#include <thread>

class ThreadPool {
public:
    explicit ThreadPool(size_t thread_count) : data_(std::make_shared<data>()) {
        for (size_t i = 0; i < thread_count; ++i) {
            std::thread([dataptr = data_] {
                std::unique_lock<std::mutex> lk(dataptr->mtx_);
                for (;;) {
                    if (!dataptr->tasks_.empty()) {
                        auto current = std::move(dataptr->tasks_.front());
                        dataptr->tasks_.pop();
                        lk.unlock();
                        current();
                        lk.lock();
                    } else if (dataptr->is_shutdown_) {
                        break;
                    } else {
                        dataptr->cond_.wait(lk);
                    }
                }
            }).detach();
        }
    }
    ThreadPool() = default;
    ThreadPool(ThreadPool&&) = default;
    ~ThreadPool() {
        if ((bool) data_) {
            {
                std::lock_guard<std::mutex> lk(data_->mtx_);
                data_->is_shutdown_ = true;
            }
            data_->cond_.notify_all();
        }
    }

    template <class F>
    void execute(F&& task) {
        {
            std::lock_guard<std::mutex> lk(data_->mtx_);
            data_->tasks_.emplace(std::forward<F>(task));
        }
        data_->cond_.notify_one();
    }

private:
    struct data {
        std::mutex mtx_;
        std::condition_variable cond_;
        bool is_shutdown_ = false;
        std::queue<std::function<void()>> tasks_;
    };
    std::shared_ptr<data> data_;
};

#endif //DO_NOT_YARN_THREADPOOL_H
