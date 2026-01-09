#ifndef PARALLEL_PROCESSOR_H
#define PARALLEL_PROCESSOR_H

#include <vector>
#include <thread>
#include <functional>
#include <algorithm>

class ParallelProcessor {
public:
    explicit ParallelProcessor(size_t num_threads)
        : num_threads_(std::min(num_threads, static_cast<size_t>(std::thread::hardware_concurrency()))) {
        if (num_threads_ == 0) {
            num_threads_ = std::thread::hardware_concurrency();
            if (num_threads_ == 0) num_threads_ = 1;
        }
    }

    template<typename T, typename Func>
    auto parallel_map(const std::vector<T>& data, Func func) -> std::vector<decltype(func(std::declval<T>()))> {
        using U = decltype(func(std::declval<T>()));
        if (data.empty()) return {};

        std::vector<U> result(data.size());
        std::vector<std::thread> threads;

        size_t chunk_size = data.size() / num_threads_;
        size_t remainder = data.size() % num_threads_;

        size_t start = 0;
        for (size_t i = 0; i < num_threads_; ++i) {
            size_t end = start + chunk_size + (i < remainder ? 1 : 0);

            threads.emplace_back([&data, &result, &func, start, end]() {
                for (size_t j = start; j < end; ++j) {
                    result[j] = func(data[j]);
                }
            });

            start = end;
        }

        for (auto& thread : threads) {
            thread.join();
        }

        return result;
    }

    size_t get_num_threads() const {
        return num_threads_;
    }

private:
    size_t num_threads_;
};

#endif
