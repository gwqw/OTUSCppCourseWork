#pragma once

#include <queue>
#include <set>
#include <stdexcept>
#include <memory>
#include <utility>
#include <mutex>

template <class T>
class ObjectPool {
public:
    static ObjectPool& getSingletone() {
        static ObjectPool singleObjectPool;
        return singleObjectPool;
    }
    ObjectPool(const ObjectPool&) = delete;
    ObjectPool& operator=(const ObjectPool&) = delete;

    std::shared_ptr<T> allocate();

    void deallocate(std::shared_ptr<T>&& object);

private:
    std::queue<std::shared_ptr<T>> free_;
    std::set<T*> allocated_;
    std::mutex mtx_;

    ObjectPool() = default;
};

template <typename T>
std::shared_ptr<T> ObjectPool<T>::allocate() {
    std::lock_guard<std::mutex> lk(mtx_);
    if (free_.empty()) {
        free_.push(std::make_shared<T>());
    }
    auto result = std::move(free_.front());
    free_.pop();
    allocated_.insert(result.get());
    return result;
}

template <typename T>
void ObjectPool<T>::deallocate(std::shared_ptr<T>&& object) {
    std::lock_guard<std::mutex> lk(mtx_);
    if (allocated_.find(object.get()) == allocated_.end()) {
        throw std::invalid_argument("This object has not been allocated by object pool");
    }
    allocated_.erase(object.get());
    free_.push(std::move(object));
}
