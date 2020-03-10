#pragma once

#include <queue>
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

    template <typename ... Args>
    std::shared_ptr<T> construct(Args&&... args);

    void deallocate(std::shared_ptr<T>&& object);

private:
    std::queue<std::shared_ptr<T>> free_;
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
    return result;
}

template <typename T>
template <typename ... Args>
std::shared_ptr<T> ObjectPool<T>::construct(Args&&... args) {
    std::lock_guard<std::mutex> lk(mtx_);
    if (free_.empty()) {
        free_.push(std::make_shared<T>(std::forward<Args>(args)...));
    }
    auto result = std::move(free_.front());
    free_.pop();
    return result;
}

template <typename T>
void ObjectPool<T>::deallocate(std::shared_ptr<T>&& object) {
    std::lock_guard<std::mutex> lk(mtx_);
    free_.push(std::move(object));
}
