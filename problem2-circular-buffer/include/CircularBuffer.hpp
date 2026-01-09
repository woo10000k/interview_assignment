#ifndef CIRCULAR_BUFFER_HPP
#define CIRCULAR_BUFFER_HPP

#include <vector>
#include <stdexcept>
#include <algorithm>
#include <numeric>
#include <iterator>

template<typename T>
class CircularBuffer {
public:
    class Iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        Iterator(CircularBuffer* buffer, size_t index)
            : buffer_(buffer), index_(index) {}

        reference operator*() const {
            return buffer_->buffer_[(buffer_->head_ + index_) % buffer_->capacity_];
        }

        pointer operator->() const {
            return &(buffer_->buffer_[(buffer_->head_ + index_) % buffer_->capacity_]);
        }

        Iterator& operator++() {
            ++index_;
            return *this;
        }

        Iterator operator++(int) {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        bool operator==(const Iterator& other) const {
            return index_ == other.index_ && buffer_ == other.buffer_;
        }

        bool operator!=(const Iterator& other) const {
            return !(*this == other);
        }

    private:
        CircularBuffer* buffer_;
        size_t index_;
    };

    class ConstIterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = const T*;
        using reference = const T&;

        ConstIterator(const CircularBuffer* buffer, size_t index)
            : buffer_(buffer), index_(index) {}

        reference operator*() const {
            return buffer_->buffer_[(buffer_->head_ + index_) % buffer_->capacity_];
        }

        pointer operator->() const {
            return &(buffer_->buffer_[(buffer_->head_ + index_) % buffer_->capacity_]);
        }

        ConstIterator& operator++() {
            ++index_;
            return *this;
        }

        ConstIterator operator++(int) {
            ConstIterator tmp = *this;
            ++(*this);
            return tmp;
        }

        bool operator==(const ConstIterator& other) const {
            return index_ == other.index_ && buffer_ == other.buffer_;
        }

        bool operator!=(const ConstIterator& other) const {
            return !(*this == other);
        }

    private:
        const CircularBuffer* buffer_;
        size_t index_;
    };

    explicit CircularBuffer(size_t capacity)
        : buffer_(capacity), head_(0), tail_(0), size_(0), capacity_(capacity) {
        if (capacity == 0) {
            throw std::invalid_argument("Capacity must be greater than 0");
        }
    }

    CircularBuffer(const CircularBuffer& other)
        : buffer_(other.buffer_), head_(other.head_), tail_(other.tail_),
          size_(other.size_), capacity_(other.capacity_) {}

    CircularBuffer(CircularBuffer&& other) noexcept
        : buffer_(std::move(other.buffer_)), head_(other.head_), tail_(other.tail_),
          size_(other.size_), capacity_(other.capacity_) {
        other.head_ = 0;
        other.tail_ = 0;
        other.size_ = 0;
        other.capacity_ = 0;
    }

    CircularBuffer& operator=(const CircularBuffer& other) {
        if (this != &other) {
            buffer_ = other.buffer_;
            head_ = other.head_;
            tail_ = other.tail_;
            size_ = other.size_;
            capacity_ = other.capacity_;
        }
        return *this;
    }

    CircularBuffer& operator=(CircularBuffer&& other) noexcept {
        if (this != &other) {
            buffer_ = std::move(other.buffer_);
            head_ = other.head_;
            tail_ = other.tail_;
            size_ = other.size_;
            capacity_ = other.capacity_;
            other.head_ = 0;
            other.tail_ = 0;
            other.size_ = 0;
            other.capacity_ = 0;
        }
        return *this;
    }

    ~CircularBuffer() = default;

    void push_back(const T& item) {
        buffer_[tail_] = item;

        if (size_ < capacity_) {
            size_++;
        } else {
            head_ = (head_ + 1) % capacity_;
        }

        tail_ = (tail_ + 1) % capacity_;
    }

    void pop_front() {
        if (empty()) {
            throw std::runtime_error("Buffer is empty");
        }

        head_ = (head_ + 1) % capacity_;
        size_--;
    }

    T& front() {
        if (empty()) {
            throw std::runtime_error("Buffer is empty");
        }
        return buffer_[head_];
    }

    const T& front() const {
        if (empty()) {
            throw std::runtime_error("Buffer is empty");
        }
        return buffer_[head_];
    }

    T& back() {
        if (empty()) {
            throw std::runtime_error("Buffer is empty");
        }
        return buffer_[(tail_ + capacity_ - 1) % capacity_];
    }

    const T& back() const {
        if (empty()) {
            throw std::runtime_error("Buffer is empty");
        }
        return buffer_[(tail_ + capacity_ - 1) % capacity_];
    }

    size_t size() const {
        return size_;
    }

    size_t capacity() const {
        return capacity_;
    }

    bool empty() const {
        return size_ == 0;
    }

    Iterator begin() {
        return Iterator(this, 0);
    }

    Iterator end() {
        return Iterator(this, size_);
    }

    ConstIterator begin() const {
        return ConstIterator(this, 0);
    }

    ConstIterator end() const {
        return ConstIterator(this, size_);
    }

private:
    std::vector<T> buffer_;
    size_t head_;
    size_t tail_;
    size_t size_;
    size_t capacity_;
};

#endif
