//
// Created by jipeng on 5/25/18.
//

#ifndef DO_NOT_YARN_CONCURRENTBLOCKINGQUEUE_H
#define DO_NOT_YARN_CONCURRENTBLOCKINGQUEUE_H


#include <mutex>
#include <queue>
#include <memory>

template<class T>
class ConcurrentBlockingQueue
{
private:
    mutable std::mutex mut;
    std::queue<std::shared_ptr<T>> data_queue;
    std::condition_variable data_con;
public:
    ConcurrentBlockingQueue(){}
    ConcurrentBlockingQueue(ConcurrentBlockingQueue const& other)
    {
        std::lock_guard<std::mutex> lk(other.mut);
        data_queue = other.data_queue;
    }
    void push(T tValue)
    {
        std::shared_ptr<T> data(std::make_shared<T>(std::move(tValue)));
        std::lock_guard<std::mutex> lk(mut);
        data_queue.push(data);
        data_con.notify_one();
    }
    void wait_and_pop(T& tValue)
    {
        std::unique_lock<std::mutex> lk(mut);
        data_con.wait(lk,[this]{return !data_queue.empty();});
        tValue = std::move(*data_queue.front());
        data_queue.pop();
    }
    std::shared_ptr<T> wait_and_pop()
    {
        std::unique_lock<std::mutex> lk(mut);
        data_con.wait(lk,[this]{return !data_queue.empty();});
        std::shared_ptr<T> ret (std::make_shared<T>(data_queue.front()));
        data_queue.pop();
        return ret;
    }
    bool try_pop(T& tValue)
    {
        std::lock_guard<std::mutex> lk(mut);
        if(data_queue.empty())
            return false;
        tValue = std::move(*data_queue.front());
        data_queue.pop();
        return true;
    }
    std::shared_ptr<T> try_pop()
    {
        std::lock_guard<std::mutex> lk(mut);
        if(data_queue.empty())
            return std::shared_ptr<T>();
        std::shared_ptr<T> ret(std::make_shared(data_queue.front()));
        data_queue.pop();
        return ret;
    }
    bool empty() const
    {
        std::lock_guard<std::mutex> lk(mut);
        return data_queue.empty();
    }
};



#endif //DO_NOT_YARN_CONCURRENTBLOCKINGQUEUE_H
